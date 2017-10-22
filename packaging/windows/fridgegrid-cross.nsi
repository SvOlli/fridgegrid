; fridgegrid-cross.nsi
;
; hacked by Sven Oliver Moll using example2.nsi as a template
;
; distributed under the terms of the GNU General Public License (GPL)
; available at http://www.gnu.org/licenses/gpl.html
;
;--------------------------------

; The name of the installer
Name "fridgegrid"

; The file to write
OutFile "..\..\..\fridgegrid-installer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\fridgegrid

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\fridgegrid" "Install_Dir"

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
Section "fridgegrid (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  !include ..\..\build\packaging\files-install.nsh
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\fridgegrid "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\fridgegrid" "DisplayName" "fridgegrid"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\fridgegrid" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\fridgegrid" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\fridgegrid" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\fridgegrid"
  CreateShortCut "$SMPROGRAMS\fridgegrid\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\fridgegrid\fridgegrid.lnk" "$INSTDIR\fridgegrid.exe" "" "$INSTDIR\fridgegrid.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\fridgegrid"
  DeleteRegKey HKLM SOFTWARE\fridgegrid

  ; Remove files and uninstaller
  !include ..\..\build\packaging\files-delete.nsh
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\fridgegrid\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\fridgegrid"
  RMDir "$INSTDIR"

SectionEnd
