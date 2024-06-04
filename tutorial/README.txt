todo:

neuralnetwork. h and . cpp
coding tips:
argument list missing after assumed function template: template file not even included!! (vectn)
go past all TODO's

serializevalue throwing static assert: fp changed so long double used instead of double (no serialization method for long double

redefinition different basic types: you used auto
redefinition: pragma once missing

linker error: multiple definition of type_info:
make sure to only link std-c++ statically in release mode!

linker error unresolved external symbol? make inline
base class undefined: circular dependencies
gzip reinterpret cast error: define ZLIB_CONST before inclusion

linker error already defined in obj: you #included a '.cpp' file instead of a '.h' file
or you initialized an extern in the declaration
compilation slow: turn 'multi procession compilation' to 'yes' in the project settings. not in the soluition settings!
unresolved external: cpp file not added to solution?


unresolved external symbol (linker errors) with sfml? did you check if someone #set(SFML_BUILD_AUDIO FALSE) ?
also, 
target_link_libraries(${PROJECT_NAME} PRIVATE 
sfml-audio
sfml-network
sfml-graphics
sfml-system
sfml-window
)

if you need a library like zlib, you can also install it with vcpkg.
vcpkg install zlib
it will give instructions on what to put in cmakelists.txt!

undefined reference while reference is defined: recompile cmakelists.txt. you probably added a .cpp file

'uint32_t' is not a member of 'std':
#include <cstdint> not <stdint> (stdint has no std:: namespace in front of it)

no #include errors:
search for this setting: toggle intellisense engine fallback on include errors

Cyclic build dependency detected: IItemComparable.h depends on slabType.h depends on IItemComparable.h
exclude the files from the project, then reinclude them

installer not creating new files: maybe the installer script you're looking at is in another location!

same goes for connection issues: maybe your project isn't updated enough

no copy constructor available or copy constructor is declared 'explicit': don't only check return values. check all passed values! in lambdas they should have an '&' in front

selector issues: 
- a socket can't be in multiple selectors!
- when disconnecting a socket, first remove it from it's selector!

delete gets called randomly: do NOT copy, but create a reference! const &

why is serializeValue not const? because when const is used, serializeValue() can not read.
why is it not a template then with read or write? because template functions can't be overloaded or virtual

couldn't deduce template parameter:
maybe the function calls a subfunction which requires template parameters which it can't deduce

access violation, but only in release mode? maybe you are keeping a const reference as a class member while in release the original variable gets optimized away
another glitch, but only in release mode? check the warnings. take them seriously!
for example, enchantment glitch was fixed when i fixed a warning about a possibly dangling reference

loottables not the same as displayed: other loottable folder. check path!

warning conversion from 'this' to 'that' with constexpr variable: the line of the error is not the correct line. the error was created where it was used

issue with shared ptr destruction throwing: use std::make_shared<t> instead of std::shared_ptr<t>(new t())

to move a file while keeping it included:
in the solution explorer, click on the 'show all files' button. then it will show the folder view without having to switch views.

setup project:
https://github.com/lutrarutra/SFML-Project-Template/blob/master/README.md

problems with mingw not found in path:
edit environment variables for your account (see link above)

Unable to determine what CMake generator to use. Please install or configure a preferred generator, or update settings.json, your Kit configuration or PATH variable. Error: No usable generator found:
https://stackoverflow.com/questions/70197831/unable-to-determine-what-cmake-generator-to-use-vs-code-windows-10

how to deploy / create an installer:

https://learn.microsoft.com/en-us/cpp/ide/walkthrough-deploying-your-program-cpp?view=msvc-170

how to add a folder to the installer:
https://stackoverflow.com/questions/46493023/add-custom-folder-with-microsoft-visual-studio-installer-projects
In the File System view in the setup project, right click that File System node, then Add Special Folder, then Custom Folder. In the properties window of that custom folder in DefaultLocation put [WindowsVolume] (it's not in the drop-down list).

cannot open output file: permission denied.
the output file is in use by another program (are you double launching?)

how to set up precompiled headers:

https://stackoverflow.com/questions/12434123/how-to-include-the-stdafx-h-from-the-root-directory

how to disable warnings from external headers, including imageloader.h:
https://developercommunity.visualstudio.com/t/pragma-warningpush-0-no-longer-works/1433518

to include sfml:
use vcpkg (click on an include directive to see)

a window is attached to a thread. use that thread only!
do the rendering preferrably on the main thread, while the game itself can be on another thread.

do NOT put code in your onedrive documents folder! once it's full, it basically plays a ransom game where you either heve to buy more storage or lose your files after 30 days.
to fix, delete the ipch files and the installer EXE and empty the release and debug output folders

debug heap (memory, access violation etc) errors: use application verifier

memory error after setting a pointer of (for example, previously) arraynd.baseArray to another object: after initializing, it should always be moved (arr&&)
aka implement a proper move constructor which copies the pointer and empties the old struct!

memory error with NBT values:
did you forget to initialize a pointer to which you are receiving with either nullptr or an existing array?

how to create an installer: use inno setup. it's very simple!

dll not found in visual studio itself, out of nowhere: clean and rebuild solution

how to manage nuget packages: project -> manage nuget packages (doesn't seem to have much for c++)

etw collection is currently in progress:
https://stackoverflow.com/questions/16724041/cannot-start-concurrency-visualizer-in-visual-studio-2012-got-error-unable-to

include file problems: specify include folder both in the c_cpp_properties.json and using include_directories() in CMakeLists.txt

when having an empty build folder:
run CMake: configure

no builtin-baseline:
https://stackoverflow.com/questions/75184879/builtin-baseline-in-vcpkg-for-installing-franka-matlab
run vcpkg x-update-baseline --add-initial-baseline

when installing vcpkg packages, browse on https://vcpkg.io/en/packages to find them and their version!

follow instructions of the packages! look what INCLUDE_DIRS they need on the command line!

fatal error RC1107: invalid usage; use RC /? for Help:
you included resource (.rc) files?

in extensions: set extra arguments to -G "MinGW Makefiles"

for now disable the automatic recompile

also set additional compilers to search for to C:\msys64\urct64\bin

powershell problems and debugger not breaking when std::terminate() is called:
https://github.com/microsoft/vscode-cpptools/issues/9677

old make command:
ninja.exe CXXFLAGS=\"-g\" --version -j12"

https://code.visualstudio.com/docs/cpp/cpp-debug:
 To debug your Cygwin or MinGW application, add the miDebuggerPath property and set its value to the location of the corresponding gdb.exe for your Cygwin or MinGW environment.

For example:

    "miDebuggerPath": "c:\\mingw\\bin\\gdb.exe"

https://code.visualstudio.com/docs/cpp/cpp-debug:
Debugging
Windows:

GDB on Cygwin and MinGW cannot break a running process. To set a breakpoint when the application is running (not stopped under the debugger), or to pause the application being debugged, press Ctrl-C in the application's terminal.
GDB on Cygwin cannot open core dumps.

(changed to clang)

add support for mingw breakpoints:
MSYS2/Cygwin/MinGW/Clang support extension

add ninja:
mingw-w64-x86_64-ninja
to msys2

cmake set to debug:
"command": "cmake .. -G Ninja -DCMAKE_BUILD_TYPE:STRING=Release"

these loops are nested loops!
    do
        while (startRendering->get_future().get())
        {
            // wait until signal to start processing is set

            startRendering = std::make_shared<std::promise<bool>>();

            texture &graphics = *buffer[0];
            mainForm->render(cveci2(0, 0), graphics);
            // Draw graphics->colors to window

            graphics.switchChannels(graphics.baseArray, 0, 2);
            finishedRendering->set_value();
        }
    while (true);

error in a member function with an almost empty stack trace:
make sure to std::bind to a reference, not a copy!
socket->sendPacketThread = new std::thread(std::bind(&tcpSerializer::sendPacket, &socket->s));
&socket->s instead of socket->s

problem with a base function not behaving the same way as the parent function: the parent function is derived!!

calling a member function calls a member function of another type:
maybe the function which contains the call is called using a std::function pointer and is bound to the wrong object.
example:
    sideSlider->addEventHandlers(&listControl::sideSliderOnValueChanged,
                                 sideSlider->onValueChanged);
woops! now the object which gets used as class pointer is the sideslider, not this!

analyze apk file:
android studio -> build -> analyze apk...

ignoreassetspattern:
https://stackoverflow.com/questions/59158942/make-android-copy-all-from-next-asset-folder-via-aaptoptions

problems with concepts:
try to make them use constexpr evaluation only (no functions). idk why but they don't seem to work

common macro errors:
- use COMMA instead of ,
- make sure to put a semicolon (;) behind each line!

there are no arguments to that depend on a template parameter:
typedef the base class as 'base'. then call the base functions using 'base::'

linker.exe exits with code 5:
remove all other compiler paths from PATH

debug spitting library unload messages:
seems to have to do with gl context creation. create as few as possible and reuse contexts!

facts about sfml:
rendertextures don't have their own contexts, they use the current context and bind to that

openCV doesn't support live streams or writing to a byte stream!

nbtserializer problems:
string can only store a string of length short.maxvalue!

setactive() must also be called when you don't use openGL but use multithreading

it's better to have all openGL rendering in one thread! multiple rendering threads using OpenGL won't give any speedup, if they even work!
for now, let's just use the CPU for rendering. later we could try to create a single render thread (which renders for server and client) and send all the data to that thread which executes it.

ERROR: Unable to start debugging. GDB exited unexpectedly.
to debug this, try running the exe by double clicking on it (not with a debugger)

ERROR: Unable to start debugging. Unexpected GDB output from command "-exec-run". During startup program exited with code 0xc0000135.
The program 'C:\Users\Eigenaar\Documents\Github\MedievalSurvival\build\a.exe' has exited with code 0 (0x00000000).

see what happens when you run the .exe without a debugger (just by clicking on it)

c++ standard:
change in CMakeLists.txt : 
set(CMAKE_CXX_STANDARD ##) with ## being version number

do not set and use a map key in one line! somehow it doesn't work.

also change in c_cpp_properties.json

type/value mismatch: f.e. use std::remove_const_t instead of std::remove_const::value

error with std::chrono::system_clock::now: use steady_clock everywhere 

vs code intellisense lagging: another extension is causing it. disable all extensions and see what extension causes it
header files are hard to find for vs code intellisense or go to definition isn't working that well:
go to the configuration. maybe it can't find one of the paths you provided.

Android studio:
renamed something, but databinding gives errors:
- rebuild
- file-> invalidate caches and restart

libmedievalsurvival.so not found:
so files are for example in app\build\intermediates\cxx\Debug\235d3j5x\obj\arm64-v8a

ANativeActivity_OnCreate unresolved symbol while the basic functions of sfml get loaded: make sure to link sfml-main using -Wl --whole-archive!

error when calling something from JNI: check logcat!!

cannot open a certain folder in the device explorer: make sure to simulate a device without a google system image (just use a normal android image like upsidedowncake)

android studio not updating info about c++ function template: delete the function call (ctrl + x) and let it reappear (ctrl + v or ctrl + z)

error running app: build type isn't debuggable: when built in release mode. select build - > build variants - > debug to debug

disable the app being restarted when rotating:
things you add to configchanges will not close the app when they happen.
            android:configChanges="orientation|screenSize|screenLayout|keyboardHidden"

segmentation fault:
make sure to capture things in lambdas as (const) references where possible!

all files appear to be rolled back:
check if you're in the right directory and if you're the right user!!

TODO:

maybe add vcpkg-configuration.json for version consistency (see https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd)

add cmake-toolchain for cross-compile

bonemeal particles
creeper ontploft in gmc
knockback in gmc
fix textures: when you summon a blaze in the end, the texture changes
make the world save when you close the game / autosave
gui textures don't work when they are 2x as big or small
ender dragon dying beams uses worldrandom instead of currentrandom
rename some conditions to have 'condition' behind it
place ruined portal function 
stemPlantGrowthStageCount
gameModeID.h name
save enchantment random in player data
middle click in creative: pick block
stop main menu music from playing when in game
dropping items
wind sounds
legs swinging too fast
wind too fast
enderman should become mad when slapped (anger sound)
remove AI

rendering should NEVER change the world, because it is multithreaded! for example:
a player cannot be swapping slots in the render() function!
