import com.android.build.api.dsl.AndroidResources
import org.gradle.internal.impldep.org.junit.experimental.categories.Categories.CategoryFilter.exclude

plugins {
    alias(libs.plugins.androidApplication)
    alias(libs.plugins.jetbrainsKotlinAndroid)
}

android {
    ndkVersion = "26.2.11394342"
    namespace = "com.johnheikens.medievalsurvival"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.johnheikens.medievalsurvival"
        minSdk = 26
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        manifestPlaceholders["libNameSuffix"] = ""
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
            signingConfig = signingConfigs.getByName("debug")
            isDebuggable = true
        }
        debug{
            manifestPlaceholders["libNameSuffix"] = "-d"
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
    externalNativeBuild {
        cmake {
            //path = file("src/main/cpp/CMakeLists.txt")
            path = file("../../CMakeLists.txt")
            version = "3.22.1"
        }
    }
    buildFeatures {
        viewBinding = true
    }
    packaging {
        jniLibs {
            useLegacyPackaging = true
        }
    }
    //applicationVariants.all { variant ->
    //    if (variant.buildType.name == "release") {
    //        variant.mergeAssetsProvider.configure {
    //            doLast {
    //                delete( fileTree(outputDir))
    //            }
    //        }
    //    }
    //}
    androidResources() {
        //ignoreAssetsPatterns.add("!data/saves/**")
        //ignoreAssetsPatterns.add("!**")
        //ignoreAssetsPatterns.add("!*.dat")
    }
    buildTypes {
        release {
            androidResources {
                ignoreAssetsPattern = "playerdata.dat:saves:screenshots:BDCraft Sound Pack:4x4:4thful:randomized loot:*.class"
            }
        }
    }

    //sourceSets {
    //    named("main") {
//
    //        resources {
    //            srcDirs("src/main/assets")
    //            exclude()
    //        }
    //        assets.srcDir("src/main/assets").
    //        //assets fileTree(projectDir).matching {
    //        //    include '**/src/main/java'
    //        //}
    //    }
    //}

    //sourceSets {
    //    getByName("main") {
    //        jni {
    //            srcDirs("src\\main\\jni", "src\\main\\jni")
    //        }
    //    }
    //}
    //sourceSets {
    //    getByName("main") {
    //        jniLibs.srcDirs("../sfml/lib")
    //    }
    //}
    //defaultConfig{
    //    ndk{//https://tgui.eu/tutorials/0.9/android-sfml/
    //
    //        abiFilters += listOf("armeabi-v7a", "x86", "arm64-v8a", "x86_64")//, "arm64-v8a", "x86_64"
    //    }
    //
    //}

    //splits {
    //    abi {
    //        enable true
    //        reset()
    //        include "armeabi-v7a", "x86", "arm64-v8a", "x86_64"
    //        universalApk true
    //    }
    //}

    //sourceSets {
    //    mainActivity {
    //        assets.srcDirs = ['src/main/assets']
    //    }
    //}
}

dependencies {
    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    implementation(libs.androidx.constraintlayout)
    implementation(libs.androidx.navigation.fragment.ktx)
    implementation(libs.androidx.navigation.ui.ktx)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
}