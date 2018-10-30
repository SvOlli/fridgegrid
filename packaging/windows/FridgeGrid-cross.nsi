; fridgegrid-cross.nsi
;
; hacked by Sven Oliver Moll using example2.nsi as a template
;
; distributed under the terms of the GNU General Public License (GPL)
; available at http://www.gnu.org/licenses/gpl.html
;
;--------------------------------

; The name of the installer
Name "FridgeGrid"

; The file to write
OutFile "..\..\..\FridgeGrid-Installer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\FridgeGrid

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\FridgeGrid" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

; Set maximum compression
SetCompressor /SOLID lzma

; License to use
LicenseData ..\..\LICENSE.TXT

;--------------------------------

; Pages

Page license
;Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "FridgeGrid (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Put file there
  !include ..\..\build-release\packaging\files-install.nsh

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\FridgeGrid "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FridgeGrid" "DisplayName" "FridgeGrid"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FridgeGrid" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FridgeGrid" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FridgeGrid" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\FridgeGrid"
  CreateShortCut "$SMPROGRAMS\FridgeGrid\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\FridgeGrid\FridgeGrid.lnk" "$INSTDIR\FridgeGrid.exe" "" "$INSTDIR\FridgeGrid.exe" 0

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FridgeGrid"
  DeleteRegKey HKLM SOFTWARE\FridgeGrid

  ; Remove files and uninstaller
  !include ..\..\build-release\packaging\files-delete.nsh
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\FridgeGrid\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\FridgeGrid"
  RMDir "$INSTDIR"

SectionEnd
