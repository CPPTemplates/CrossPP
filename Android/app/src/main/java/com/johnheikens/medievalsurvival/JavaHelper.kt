package com.johnheikens.medievalsurvival

import android.R
import android.app.Activity
import android.content.Context
import android.content.res.AssetManager
import android.view.Gravity
import android.view.ViewGroup
import android.widget.FrameLayout
import android.widget.ProgressBar
import java.io.BufferedReader
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.io.InputStreamReader
import java.io.OutputStream
import java.nio.charset.StandardCharsets
import java.nio.file.Files
import java.nio.file.Paths


class JavaHelper {
    companion object {


        @JvmStatic
        fun initializeContext(activity: Activity): String {
            print("initializing the context")
            val assetDir = "data" // Assuming "data" is the directory you want to copy recursively
            val destinationDir = File(activity.filesDir, assetDir)//context.cacheDir

            var shouldCopy = false
            if (!destinationDir.exists()) {
                shouldCopy = true
                print("didn't find the data folder")
            } else {
                val extractedVersionFilePath =
                    Paths.get(activity.filesDir.toString(), assetDir, "version.txt")
                if (File(extractedVersionFilePath.toUri()).exists()) {

                    val extractedVersion =
                        Integer.valueOf(Files.readAllLines(extractedVersionFilePath)[0])

                    val assetStream =
                        activity.assets.open(Paths.get(assetDir, "version.txt").toString());

                    val sb = StringBuilder()
                    val br = BufferedReader(InputStreamReader(assetStream, StandardCharsets.UTF_8))
                    var str: String?
                    while (br.readLine().also { str = it } != null) {
                        sb.append(str)
                    }
                    br.close()
                    if (sb.isEmpty()) {
                        shouldCopy = true;
                        print("couldn't extract asset version")
                    } else {
                        val assetVersion = Integer.valueOf(sb.toString());
                        shouldCopy = assetVersion > extractedVersion;
                    }
                } else {
                    shouldCopy = true
                    print("didn't find version.txt")
                }
            }

            //activity.runOnUiThread{
            //    var pb = ProgressBar(activity, null, R.attr.progressBarStyleHorizontal);
            //    pb.setPadding(20, 10, 10, 10);
            //    pb.isIndeterminate = false;
            //    pb.progress = 0;
            //    pb.scrollBarStyle = ProgressBar.SCROLLBARS_OUTSIDE_INSET;
            //    pb.setMax(100);
            //    pb.visibility = ProgressBar.VISIBLE;
//
            //    // Set the layout parameters for the ProgressBar
            //    val layoutParams = FrameLayout.LayoutParams(
            //        ViewGroup.LayoutParams.WRAP_CONTENT,
            //        ViewGroup.LayoutParams.WRAP_CONTENT
            //    )
            //    layoutParams.gravity = Gravity.CENTER
            //    val rootView = activity.findViewById<ViewGroup>(android.R.id.content)
            //    rootView?.addView(pb, layoutParams)
            //};
            //activity.setVisible(true);
            //var pb = ProgressBar(activity, null, R.attr.progressBarStyleHorizontal);
            //pb.setPadding(20, 10, 10, 10);
            //pb.isIndeterminate = false;
            //pb.progress = 0;
            //pb.scrollBarStyle = ProgressBar.SCROLLBARS_OUTSIDE_INSET;
            //pb.setMax(100);
            //pb.visibility = ProgressBar.VISIBLE;
//
            //// Set the layout parameters for the ProgressBar
            //val layoutParams = FrameLayout.LayoutParams(
            //    ViewGroup.LayoutParams.WRAP_CONTENT,
            //    ViewGroup.LayoutParams.WRAP_CONTENT
            //)
            //layoutParams.gravity = Gravity.CENTER
            //val rootView = activity.findViewById<ViewGroup>(android.R.id.content)
            //activity.setContentView(pb);
            //rootView?.addView(pb, layoutParams)
            Thread.sleep(1000);

            //when the 'data' folder is not found, then we extract the data folder.
            if (shouldCopy) {


//
                //// Add the ProgressBar to the root layout of the activity
//


                //print("extracting assets...")
                //val layout: RelativeLayout =
                //    findViewById(R.id.display) //specify here Root layout Id
//
                //val dialog = ProgressDialog(context)
                //dialog.setMessage("message")
                //dialog.setCancelable(false)
                //dialog.setInverseBackgroundForced(false)
                //dialog.show()
                copyAssetsToCache(activity, assetDir, destinationDir)
            }
            return destinationDir.toString();
        }

        @JvmStatic
        fun copyAssetsToCache(context: Context, assetDir: String, destinationDir: File) {
            val assetManager = context.assets
            try {
                val assets = assetManager.list(assetDir)
                if (!assets.isNullOrEmpty()) {
                    // If the assetDir is a directory
                    destinationDir.mkdirs()
                    for (asset in assets) {
                        val assetPath = assetDir + File.separator + asset
                        val destinationPath = destinationDir.absolutePath + File.separator + asset
                        val newDestinationDir = File(destinationDir, asset)

                        copyAssetsToCache(context, assetPath, newDestinationDir)
                        //copyAssetFile(assetManager, assetPath, destinationPath)
                    }
                } else {
                    // If the assetDir is a file
                    copyAssetFile(assetManager, assetDir, destinationDir.path)
                    //val subDirs = assetManager.list(assetDir)
                    //if (subDirs != null) {
                    //    for (subDir in subDirs) {
                    //        val newAssetDir = assetDir + File.separator + subDir
                    //        val newDestinationDir = File(destinationDir, subDir)
                    //        copyAssetsToCache(context, newAssetDir, newDestinationDir)
                    //    }
                    //}
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }

        @Throws(IOException::class)
        @JvmStatic
        private fun copyAssetFile(
            assetManager: AssetManager,
            assetPath: String,
            destinationPath: String
        ) {
            val `in` = assetManager.open(assetPath)
            val out: OutputStream = FileOutputStream(destinationPath)
            val buffer = ByteArray(1024)
            var read: Int
            while (`in`.read(buffer).also { read = it } != -1) {
                out.write(buffer, 0, read)
            }
            `in`.close()
            out.close()
        }
    }
}