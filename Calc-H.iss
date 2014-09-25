; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{1A516438-9674-40B5-AE5A-D17756F3CABD}
AppName=ひらがな電卓 Calc-H
AppVerName=ひらがな電卓 Calc-H 0.8.2
AppPublisher=片山博文MZ
AppPublisherURL=http://katahiromz.web.fc2.com/
AppSupportURL=http://katahiromz.web.fc2.com/
AppUpdatesURL=http://katahiromz.web.fc2.com/
DefaultDirName={pf}\Calc-H
DefaultGroupName=ひらがな電卓 Calc-H
AllowNoIcons=yes
LicenseFile=License.txt
OutputDir=.
OutputBaseFilename=calch_setup
SetupIconFile=Calc-H.ico
Compression=lzma
SolidCompression=yes
UninstallDisplayIcon={app}\Calc-H-GUI.exe

[Languages]
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "ReadMe.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "License.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "Examples.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "Calc-H-GUI.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "Calc-H-CUI.exe"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\ひらがな電卓 Calc-H (GUI)"; Filename: "{app}\Calc-H-GUI.exe"
Name: "{group}\ひらがな電卓 Calc-H (CUI)"; Filename: "{app}\Calc-H-CUI.exe"
Name: "{group}\ReadMe.txt"; Filename: "{app}\ReadMe.txt"
Name: "{group}\License.txt"; Filename: "{app}\License.txt"
Name: "{group}\Examples.txt"; Filename: "{app}\Examples.txt"
Name: "{group}\{cm:ProgramOnTheWeb,ひらがな電卓 Calc-H}"; Filename: "http://katahiromz.web.fc2.com/"
Name: "{group}\{cm:UninstallProgram,ひらがな電卓 Calc-H}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\ひらがな電卓 Calc-H (GUI)"; Filename: "{app}\Calc-H-GUI.exe"; Tasks: desktopicon
Name: "{commondesktop}\ひらがな電卓 Calc-H (CUI)"; Filename: "{app}\Calc-H-CUI.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\Calc-H-GUI.exe"; Description: "{cm:LaunchProgram,ひらがな電卓 Calc-H}"; Flags: nowait postinstall skipifsilent

