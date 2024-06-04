#include "math/rectangle/rectangletn.h"
//#include <immintrin.h> // Include SIMD header for Intel CPUs
#pragma once
/// <summary>
/// 
/// </summary>
/// <typeparam name="t"></typeparam>
/// <typeparam name="rows"></typeparam>
/// <typeparam name="cols">nested deeper</typeparam>
template<typename t, fsize_t rows, fsize_t cols>
//ordering in memory : 0 0, 0 1, 0 2, 1 0
//[0] [1] means from x to y
struct mattnxn : public vectn<vectn<t, cols>, rows>
{
	static constexpr fsize_t minimumSize = math::minimum(rows, cols);


	//standard: input = output
	constexpr mattnxn()
	{
		for (fsize_t i = 0; i < minimumSize; i++)
		{
			this->axis[i][i] = 1;
		}
	}

	template<typename t2, fsize_t cols2, fsize_t rows2>
	constexpr mattnxn(const vectn<vectn<t2, cols2>, rows2>& in) : vectn<vectn<t, cols>, rows>(in)
	{}

	inline static constexpr mattnxn empty()
	{
		return mattnxn(vectn<vectn<t, cols>, rows>());
	}

	constexpr mattnxn<t, cols, rows> transposed() const
	{
		mattnxn<t, cols, rows> result = mattnxn<t, cols, rows>();
		for (fsize_t i = 0; i < rows; i++)
		{
			for (fsize_t j = 0; j < cols; j++)
			{
				result[j][i] = this->axis[i][j];
			}
		}
		return result;
	}

	//glm method
	//https://codereview.stackexchange.com/questions/186770/4%C3%974-matrix-multiplication
	inline static constexpr mattnxn cross(const mattnxn& last, const mattnxn& first)
	{
		mattnxn mult = mattnxn::empty();//all values are 0
		fsize_t index, i, j;
		for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j) for (index = 0; index < 3; ++index) {
			mult.axis[i][index] += last.axis[j][index] * first.axis[i][j];
		}
		return mult;
	}

	//from first to last : 0 to last
	inline static mattnxn combine(const std::vector<mattnxn>& matrices)
	{
		auto i = matrices.begin();
		mattnxn out = *i;
		while (++i != matrices.end())
		{
			out = mattnxn::cross(*i, out);
		}
		return out;
	}

	constexpr typename std::enable_if<(rows > 1) && (cols > 1),
		mattnxn<t, math::maximum(rows, (fsize_t)2) - (fsize_t)1, math::maximum(cols, (fsize_t)2) - (fsize_t)1>
		>::type coFactor(cvect2<fsize_t>& pos) const
	{
		mattnxn<t, math::maximum(rows, (fsize_t)2) - (fsize_t)1, math::maximum(cols, (fsize_t)2) - (fsize_t)1>
			result =
			mattnxn<t, math::maximum(rows, (fsize_t)2) - (fsize_t)1, math::maximum(cols, (fsize_t)2) - (fsize_t)1>();

		// Looping for each element of the matrix 
		for (fsize_t row = 0, i = 0; row < rows; row++)
		{
			if (row != pos.x)
			{
				for (fsize_t col = 0, j = 0; col < cols; col++)
				{
					//  Copying into temporary matrix only those element 
					//  which are not in given row and column 
					if (col != pos.y)
					{
						result[i][j++] = this->axis[row][col];
					}
				}
				++i;
			}
		}
		return result;
	}

	//https://www.geeksforgeeks.org/adjoint-inverse-matrix/
	constexpr typename std::enable_if<((rows > 0) && (cols > 0)),
		mattnxn>::type adjoint() const
	{
		mattnxn result = mattnxn();
		if constexpr (rows == 1)
		{
			result[0][0] = 1;
		}
		else
		{
			for (fsize_t i = 0; i < rows; i++)
			{
				for (fsize_t j = 0; j < cols; j++)
				{
					// Get cofactor of A[i][j] 
					mattnxn<t,
						math::maximum(rows, (fsize_t)2) - (fsize_t)1,
						math::maximum(cols, (fsize_t)2) - (fsize_t)1>
						temp = coFactor(cvect2<fsize_t>(i, j));

					// sign of adj[j][i] positive if sum of row 
					// and column indexes is even. 
					cint& sign = ((i + j) % 2 == 0) ? 1 : -1;

					// Interchanging rows and columns to get the 
					// transpose of the cofactor matrix 
					result[j][i] = sign * temp.determinant();
				}
			}
		}
		return result;
	}

	//https://stackoverflow.com/questions/42802208/code-for-determinant-of-n-x-n-matrix#comment72802190_42804835
	constexpr t determinant() const
	{
		if constexpr (rows == 1)
		{
			return this->axis[0][0];
		}
		if constexpr (rows == 2)
		{
			return this->axis[0][0] * this->axis[1][1] - this->axis[0][1] * this->axis[1][0];
		}
		else
		{
			t result = 0;
			int sign = 1;

			for (fsize_t k = 0; k < rows; k++)
			{
				mattnxn<t, math::maximum(rows, (fsize_t)2) - (fsize_t)1, math::maximum(cols, (fsize_t)2) - (fsize_t)1>
					subMatrix = mattnxn<t, math::maximum(rows, (fsize_t)2) - (fsize_t)1, math::maximum(cols, (fsize_t)2) - (fsize_t)1>();

				for (fsize_t i = 1; i < rows; i++)
				{
					for (fsize_t aj = 0, bj = 0; aj < rows; aj++)
					{
						if (aj == k) continue;
						subMatrix[i - 1][bj] = this->axis[i][aj];
						++bj;
					}
				}

				result += sign * this->axis[0][k] * subMatrix.determinant();
				sign *= -1;
			}

			return result;
		}
	}

	//https://www.geeksforgeeks.org/adjoint-inverse-matrix/
	constexpr mattnxn inverse() const
	{
		//https://en.wikipedia.org/wiki/Inverse_matrix
		//https://www.youtube.com/watch?v=pKZyszzmyeQ
		const t& det = determinant();

		const mattnxn& adj = adjoint();

		return adj / det;
	}


	//if x++, output += getxstep
	template<fsize_t vectorSize = cols - 1>
	constexpr vectn<t, vectorSize> getStep(const axisID& axis) const
	{
		return vectn<t, vectorSize>(this->axis[(int)axis]);
	}

	//output at x 0 y 0
	constexpr typename std::enable_if<(cols > 1),
		vectn<t, math::maximum(cols, (fsize_t)2) - (fsize_t)1>> ::type getstart() const
	{
		return this->axis[rows - 1];
	}

	template<fsize_t dimensionCount>
	constexpr static typename std::enable_if <(rows > 1),
		mattnxn>::type translate(cvectn<t, dimensionCount>& add)
	{
		mattnxn result = mattnxn();
		constexpr fsize_t minimumHeight = math::minimum(dimensionCount, cols);

		for (fsize_t i = 0; i < minimumHeight; i++)
		{
			result[rows - 1][i] = add[i];
		}
		return result;
	}

	//clockwise
	//0, 90, 180 or 270
	constexpr static mattnxn rotateDegrees(cint& angle)
	{
		cint sina = math::sinDegrees(angle);
		cint cosa = math::cosDegrees(angle);
		mattnxn result = mattnxn();
		result[0][0] = cosa;
		result[1][0] = sina;
		result[0][1] = -sina;
		result[1][1] = cosa;
		return result;
	}

	inline static mattnxn rotateDegrees(const vect2<t>& rotateAround, cint& angle)
	{
		return combine({
			//first set rotation point to 0,0
			translate(-rotateAround),

			//then rotate
			rotateDegrees(angle),
			//then move it back
			translate(rotateAround)
			}
		);
	}
	//clockwise
	inline static mattnxn rotate(const t& angle)
	{
		const t& sina = sin(angle);
		const t& cosa = cos(angle);

		mattnxn result = mattnxn();
		result[0][0] = cosa;
		result[1][0] = sina;
		result[0][1] = -sina;
		result[1][1] = cosa;
		return result;
	}

	inline static mattnxn rotate(const vect2<t>& rotateAround, const t& angle)
	{
		return combine({
			//first set rotation point to 0,0
			translate(-rotateAround),

			//then rotate
			rotate(angle),
			//then move it back
			translate(rotateAround)
			}
		);
	}

	inline static mattnxn rotate3d(vect3<t> axis, const t& angle)
	{
		axis.Normalize();
		t sinr = (t)sin(angle);
		t cosr = (t)cos(angle);
		t mincos = 1 - cosr;
		//https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		return mattnxn
		{
			cosr + axis.x * axis.x * mincos,
			axis.x * axis.y * mincos - axis.z * sinr,
			axis.x * axis.z * mincos + axis.y * sinr,
			axis.y * axis.x * mincos + axis.z * sinr,
			cosr + axis.y * axis.y * mincos,
			axis.y * axis.z * mincos - axis.x * sinr,
			axis.z * axis.x * mincos - axis.y * sinr,
			axis.z * axis.y * mincos + axis.x * sinr,
			cosr + axis.z * axis.z * mincos
		};
		//axis.Normalize();
		//Matrix lhs = axis;
		//Matrix cross = Cross(lhs, matrix);
		//return cos * matrix + sin * cross;
	}

	template<fsize_t dimensionCount>
	inline static constexpr mattnxn scale(const vectn<t, dimensionCount> scalar)
	{
		constexpr fsize_t minimumSizeAndVector = math::minimum(dimensionCount, minimumSize);
		constexpr fsize_t maximumSizeAndVector = math::maximum(dimensionCount, minimumSize);
		mattnxn result = mattnxn();
		fsize_t i;
		for (i = 0; i < minimumSizeAndVector; i++)
		{
			result[i][i] = scalar[i];
		}
		for (; i < maximumSizeAndVector; i++)
		{
			result[i][i] = 1;
		}
		return result;
	}

	inline static constexpr mattnxn mirror(const axisID& axisIndex, const t& mirrorLocation)
	{
		vectn<t, rows - 1> translateAmount = vectn<t, rows - 1>();
		translateAmount[axisIndex] = mirrorLocation;

		vectn<t, rows - 1> scaleAmount = vectn<t, rows - 1>(1);
		scaleAmount[axisIndex] = -1;

		return cross(translate(translateAmount), //move back
			cross(scale(scaleAmount), //mirror
				translate(-translateAmount)));//move to mirror place
	}

	inline static constexpr mattnxn expandRectangle(const rectanglet2<t>& rect, const t& border)
	{
		return fromRectToRect(rect, rect.expanded(border));
	}
	template<fsize_t axisCount>
	inline static constexpr mattnxn fromRectToRect(const rectangletn<t, axisCount>& rectFrom, const rectangletn<t, axisCount>& rectTo)
	{
		const mattnxn translateFrom = translate(-rectFrom.pos0);//move rectangle from pos0 of rectFrom to 0, 0
		const mattnxn scaled = scale(rectTo.size / rectFrom.size);//resize it
		const mattnxn translateTo = translate(rectTo.pos0);//move it to the pos0 of rectTo
		return cross(translateTo, cross(scaled, translateFrom));
	}
	inline static constexpr mattnxn fromPointsToPoints(const vect2<t>& a00, const vect2<t>& a10, const vect2<t>& a01, const vect2<t>& b00, const vect2<t>& b10, const vect2<t>& b01)
	{
		//from a->barycentric point->b
		return cross(baryCentric(b00, b10, b01).inverse(), baryCentric(a00, a10, a01));
	}

	inline static constexpr mattnxn fromRectToRotatedRect(const rectanglet2<t>& rectFrom, cint& angle, const rectanglet2<t>& rectTo)
	{
		mattnxn transform = mattnxn();
		if (angle == 90 || angle == 270)
		{
			transform = fromRectToRect(rectanglet2<t>(rectFrom.x + rectFrom.w * 0.5 - rectFrom.h * 0.5, rectFrom.y + rectFrom.h * 0.5 - rectFrom.w * 0.5, rectFrom.h, rectFrom.w), rectTo);
		}
		else
		{
			transform = fromRectToRect(rectanglet2<t>(rectFrom), rectTo);
		}
		if (angle != 0)
		{
			transform = cross(transform, rotateDegrees(vect2<t>(rectFrom.x + rectFrom.w * 0.5, rectFrom.y + rectFrom.h * 0.5), angle));
		}
		return transform;
	}

	constexpr bool isStraight() const {
		return 
			(bool)this->axis[0].axis[0] ^ (bool)this->axis[1].axis[0] && 
			(bool)this->axis[0].axis[1] ^ (bool)this->axis[1].axis[1];
	}

	/*
	//https://franklinta.com/2014/09/08/computing-css-matrix3d-transforms/
	template<typename t>
	inline static mattnxn getTransform(const vect2<t> from[4], const vect2<t> to[4])
	{
		int yRowIndex = 0;

		Eigen::Matrix<t, 8, 8> A = Eigen::Matrix<t, 8, 8>();
		for (int i = 0; i < 4; i++)
		{
			const t rowXValues[8] = { from[i].x, from[i].y, 1, 0, 0, 0, -from[i].x * to[i].x, -from[i].y * to[i].x };
			A.row(yRowIndex++) = auto(rowXValues);
			const t rowYValues[8] = { 0, 0, 0, from[i].x, from[i].y, 1, -from[i].x * to[i].y, -from[i].y * to[i].y };
			A.setYRow(yRowIndex++, rowYValues);
		}
		int yRowIndex = 0;
		glm<t, 8, 1> b = mattnxn<t, 8, 1>();
			for (int i = 0; i < 4; i++)
			{
				b.values[yRowIndex++] = to[i].x;
				b.values[yRowIndex++] = to[i].x;

					# Solve A* h = b for h
					h = numeric.solve(A, b)

					H = [[h[0], h[1], 0, h[2]],
					[h[3], h[4], 0, h[5]],
					[0, 0, 1, 0],
					[h[6], h[7], 0, 1]]

					# Sanity check that H actually maps `from` to `to`
					for i in[0 ... 4]
						lhs = numeric.dot(H, [from[i].x, from[i].y, 0, 1])
						k_i = lhs[3]
						rhs = numeric.dot(k_i, [to[i].x, to[i].y, 0, 1])
						console.assert(numeric.norm2(numeric.sub(lhs, rhs)) < 1e-9, "Not equal:", lhs, rhs)
						H

						applyTransform = (element, originalPos, targetPos, callback) ->
						# All offsets were calculated relative to the document
						# Make them relative to(0, 0) of the element instead
						from = for p in originalPos
						x : p[0] - originalPos[0][0]
						y : p[1] - originalPos[0][1]
						to = for p in targetPos
						x : p[0] - originalPos[0][0]
						y : p[1] - originalPos[0][1]

						# Solve for the transform
						H = getTransform(from, to)

						# Apply the matrix3d as H transposed because matrix3d is column major order
						# Also need use toFixed because css doesn't allow scientific notation
						$(element).css
						'transform': "matrix3d(#{(H[j][i].toFixed(20) for j in [0...4] for i in [0...4]).join(',')})"
						'transform-origin' : '0 0'

						callback ? (element, H)

						makeTransformable = (selector, callback) ->
						$(selector).each(i, element) ->
						$(element).css('transform', '')

						# Add four dots to corners of `element` as control points
						controlPoints = for position in['left top', 'left bottom', 'right top', 'right bottom']
						$('<div>')
						.css
						border : '10px solid black'
						borderRadius : '10px'
						cursor : 'move'
						position : 'absolute'
						zIndex : 100000
						.appendTo 'body'
						.position
						at : position
						of : element
						collision : 'none'

						# Record the original positions of the dots
						originalPos = ([p.offset().left, p.offset().top] for p in controlPoints)

						# Transform `element` to match the new positions of the dots whenever dragged
						$(controlPoints).draggable
						start : = >
						$(element).css('pointer-events', 'none') # makes dragging around iframes easier
						drag : = >
						applyTransform(element, originalPos, ([p.offset().left, p.offset().top] for p in controlPoints), callback)
						stop: = >
						applyTransform(element, originalPos, ([p.offset().left, p.offset().top] for p in controlPoints), callback)
						$(element).css('pointer-events', 'auto')

						element

						makeTransformable('.box', (element, H) ->
							console.log($(element).css('transform'))
							$(element).html(
								$('<table>')
								.append(
									$('<tr>').html(
										$('<td>').text('matrix3d(')
									)
								)
								.append(
									for i in[0 ... 4]
										$('<tr>').append(
											for j in[0 ... 4]
												$('<td>').text(H[j][i] + if i == j == 3 then '' else ',')
												)
										)
								.append(
									$('<tr>').html(
										$('<td>').text(')')
									)
								)
												)
						)


		throw "implement with eigen";

	}
	*/

	// Compute barycentric set with respect to triangle (p00, p10, p01)
	//multiply a vect2<t> by the returned matrix and get a vect2<t>(v, w)
	inline static constexpr mattnxn baryCentric(const vect2<t>& p00, const vect2<t>& p10, const vect2<t>& p01)
	{
		cvect2<t>& v0 = p10 - p00, v1 = p01 - p00,
			v2a00 = -p00, v2a10 = vect2<t>(1, 0) - p00, v2a01 = vect2<t>(0, 1) - p00;//calculate for (0,0),(1,0),(0,1)
		const t& d00 = vect2<t>::dot(v0, v0);
		const t& d01 = vect2<t>::dot(v0, v1);
		const t& d11 = vect2<t>::dot(v1, v1);
		const t& d20a00 = vect2<t>::dot(v2a00, v0);
		const t& d21a00 = vect2<t>::dot(v2a00, v1);
		const t& d20a10 = vect2<t>::dot(v2a10, v0);
		const t& d21a10 = vect2<t>::dot(v2a10, v1);
		const t& d20a01 = vect2<t>::dot(v2a01, v0);
		const t& d21a01 = vect2<t>::dot(v2a01, v1);
		const t& denom = d00 * d11 - d01 * d01;
		const t& div = 1.0 / denom;
		const t& va00 = (d11 * d20a00 - d01 * d21a00) * div;//v at x 0 y 0
		const t& wa00 = (d00 * d21a00 - d01 * d20a00) * div;//w at x 0 y 0
		const t& va10 = (d11 * d20a10 - d01 * d21a10) * div;//v at x 1 y 0
		const t& wa10 = (d00 * d21a10 - d01 * d20a10) * div;//w at x 1 y 0
		const t& va01 = (d11 * d20a01 - d01 * d21a01) * div;//v at x 0 y 1
		const t& wa01 = (d00 * d21a01 - d01 * d20a01) * div;//w at x 0 y 1													
		//multx, multy, plus
		return mattnxn(
			va10 - va00, va01 - va00, va00,//v
			wa10 - wa00, wa01 - wa00, wa00,//w
			0, 0, 1//empty
		);
	}

	//glm method
	inline static constexpr mattnxn perspectiveFov(const t& fov, const t& width, const t& height, const t& zNear, const t& zFar)
	{
		const t& rad = fov;
		const t& h = cos(static_cast<fp>(0.5) * rad) / sin(static_cast<fp>(0.5) * rad);
		const t& w = h * height / width; ///todo max(width , Height) / min(width , Height)?

		mattnxn result = empty();
		result[0][0] = w;//the width multiplier of the screen
		result[1][1] = h;//the height multiplier of the screen
		result[2][2] = -(zFar + zNear) / (zFar - zNear);//the z multiplier
		//to bring it in range(0,1):
		result[2][3] = -static_cast<fp>(1);//the distance multiplier
		result[3][2] = -(static_cast<fp>(2) * zFar * zNear) / (zFar - zNear);//the distance substractor
		//row followed by column
		return result;
	}

	//the view matrix
	//glm method
	//https://stackoverflow.com/questions/19740463/lookat-function-im-going-crazy
	//https://stackoverflow.com/questions/21830340/understanding-glmlookat
	inline static constexpr mattnxn lookat(cvect3<t>& eye, cvect3<t>& center, cvect3<t>& up)
	{
		//Create a new coordinate system
		cvect3<t>& screenz((center - eye).normalized());//the look direction
		cvect3<t>& screenx(vect3<t>::cross(screenz, up).normalized());//the sideways direction
		cvect3<t>& screeny(vect3<t>::cross(screenz, screenx));//the upwards direction

		mattnxn result = mattnxn();
		result[0][0] = screenx.x;//the new x
		result[1][0] = screenx.y;
		result[2][0] = screenx.z;
		result[0][1] = screeny.x;//the new y
		result[1][1] = screeny.y;
		result[2][1] = screeny.z;
		result[0][2] = -screenz.x;//the new z
		result[1][2] = -screenz.y;
		result[2][2] = -screenz.z;
		result[3][0] = -vect3<t>::dot(screenx, eye);
		result[3][1] = -vect3<t>::dot(screeny, eye);
		result[3][2] = vect3<t>::dot(screenz, eye);
		return result;
	}

	constexpr rectanglet2<t> getTransformedRectangleBounds(const rectanglet2<t>& rect) const
	{
		const vect2<t> pos00 = multPointMatrix(rect.pos0);//upper left corner
		const vect2<t> pos10 = multPointMatrix(rect.pos10());
		const vect2<t> pos01 = multPointMatrix(rect.pos01());
		const vect2<t> pos11 = multPointMatrix(rect.pos1());

		rectanglet2<t> boundaries;
		boundaries.pos0 = vect2<t>(
			math::minimum(math::minimum(pos00.x, pos10.x), math::minimum(pos01.x, pos11.x)),
			math::minimum(math::minimum(pos00.y, pos10.y), math::minimum(pos01.y, pos11.y)));
		boundaries.size = vect2<t>(
			math::maximum(math::maximum(pos00.x, pos10.x), math::maximum(pos01.x, pos11.x)),
			math::maximum(math::maximum(pos00.y, pos10.y), math::maximum(pos01.y, pos11.y)))
			- boundaries.pos0;
		return boundaries;
	}

	template<fsize_t inputAxisCount = cols, fsize_t outputAxisCount = inputAxisCount>
	constexpr vectn<t, outputAxisCount> multPointMatrix(const vectn<t, inputAxisCount>& in) const;
	

	constexpr vectn<t, 2> multPointMatrix(const vectn<t, 2>& in) const;

	//multpointmatrix, but without the translate
	template<fsize_t axisCount>
	constexpr vectn<t, axisCount> multSizeMatrix(const vectn<t, axisCount>& size) const
	{
		vectn<t, axisCount> result = vectn<t, axisCount>();
		for (fsize_t toIndex = 0; toIndex < axisCount; toIndex++)
		{
			for (fsize_t fromIndex = 0; fromIndex < axisCount; fromIndex++)
			{
				result[toIndex] += size[fromIndex] * this->axis[fromIndex][toIndex];
			}
		}
		return result;
	}

	constexpr rectangle2 multRectMatrix(crectangle2& in) const
	{
		return rectangle2(multPointMatrix(in.pos0), multSizeMatrix(in.size));
	}
	constexpr rectangle2 multRotatedRectMatrix(crectangle2& in) const
	{
		return rectangle2::fromOppositeCorners(multPointMatrix(in.pos0), multPointMatrix(in.pos1()));
	}
};


typedef mattnxn<int, 3, 3> mat3x3i;

typedef mattnxn<fp, 1, 1> mat1x1;
typedef mattnxn<fp, 2, 2> mat2x2;
typedef mattnxn<fp, 3, 3> mat3x3;
typedef mattnxn<fp, 4, 4> mat4x4;

typedef const mat3x3 cmat3x3;
typedef const mat3x3i cmat3x3i;




//constexpr mat3x3 testMatrix = mat3x3(
//	vectn<vec3, 3>(
//		vec3(2, -3, 1), 
//		vec3(2, 0, -1), 
//		vec3(1, 4, 5) 
//		));
//
//constexpr vec2 multiplied = testMatrix.multPointMatrix(cvec2(0, 0));
//
//constexpr fp testDet = testMatrix.determinant();
//constexpr mat3x3 testAdj = testMatrix.adjoint();
//constexpr mat3x3 inverseTestMatrix = testMatrix.inverse();
//constexpr mat3x3 testInv = testAdj / testDet;
//static const mat2x2 testCoFactor = testMatrix.coFactor(vectn<fsize_t, 2>(0, 2));


template<typename t, fsize_t rows, fsize_t cols>
//template<fsize_t inputAxisCount, fsize_t outputAxisCount>
inline constexpr vectn<t, 2> mattnxn<t, rows, cols>::multPointMatrix(const vectn<t, 2>& in) const
{
	static_assert(rows == 3);
	static_assert(cols == 3);
	return vectn<t,2>(
		in.axis[0] * this->axis[0].axis[0] + in.axis[1] * this->axis[1].axis[0] + this->axis[2].axis[0],
		in.axis[0] * this->axis[0].axis[1] + in.axis[1] * this->axis[1].axis[1] + this->axis[2].axis[1]
	);
}


template<typename t, fsize_t rows, fsize_t cols>
template<fsize_t inputAxisCount, fsize_t outputAxisCount>
inline constexpr vectn<t, outputAxisCount> mattnxn<t, rows, cols>::multPointMatrix(const vectn<t, inputAxisCount>& in) const
{


	vectn<t, outputAxisCount> result = in.axis[0] * this->axis[0];
	fsize_t fromIndex = 1;
	for (; fromIndex < inputAxisCount; fromIndex++)
	{
		result += in.axis[fromIndex] * this->axis[fromIndex];
	}
	//the extra 1's to add to the vector
	for (; fromIndex < cols; fromIndex++)
	{
		result += this->axis[fromIndex];
	}
	return result;
}