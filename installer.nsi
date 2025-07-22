Name "Decision Maker"
OutFile "DecisionMaker_Installer.exe"
InstallDir "$PROGRAMFILES64\DecisionMaker"
RequestExecutionLevel admin

!include "MUI2.nsh"
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Section "Decision Maker"
  SetOutPath $INSTDIR
  
  # Recursively add all files and folders from the 'dist' directory
  File /r "dist\*.*"
  
  # Create shortcuts
  CreateDirectory "$SMPROGRAMS\DecisionMaker"
  CreateShortCut "$SMPROGRAMS\DecisionMaker\DecisionMaker.lnk" "$INSTDIR\DecisionMaker.exe"
  
  # Create the uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
  # Recursively remove the entire installation directory and its contents
  RMDir /r "$INSTDIR"
  
  # Remove the start menu folder
  RMDir /r "$SMPROGRAMS\DecisionMaker"
SectionEnd
