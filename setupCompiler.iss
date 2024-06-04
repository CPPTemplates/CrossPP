[Setup]
AppName=Medieval Survival
AppVersion=1.0
ArchitecturesInstallIn64BitMode=x64
DefaultDirName={commonpf}\Medieval Survival
DefaultGroupName=Medieval Survival
UninstallDisplayIcon={app}\MedievalSurvival.exe
OutputDir=Output
OutputBaseFilename=Medieval Survival - Installer
WizardResizable=yes
WizardStyle=modern

[Files]
Source: "build/Release/a.exe"; DestDir: "{app}"; DestName: "MedievalSurvival.exe"
Source: "build/Release/*.dll"; DestDir: "{app}"
//Source: "x64/release/*.pdb"; DestDir: "{app}"
Source: "Data\*"; DestDir: "{commonappdata}\JohnHeikens\Medieval Survival\data\"; Flags: recursesubdirs; Permissions: users-full; Excludes: "playerdata.dat,saves,screenshots,BDCraft Sound Pack,4x4,4thful,randomized loot,*.class";

[Icons]
Name: "{group}\Medieval Survival"; Filename: "{app}\MedievalSurvival.exe"

[Run]
Filename: "{app}\MedievalSurvival.exe"; Flags:runasoriginaluser nowait postinstall skipifsilent