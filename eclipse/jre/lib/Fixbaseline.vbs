Const HKEY_LOCAL_MACHINE = &H80000002		' Needed for Registry hive access
Const HKEY_CURRENT_USER = &H80000001		' Needed for Registry hive access
Const HKEY_USERS = &H80000003			' Needed for Registry hive access

Dim wso, fso

Set wso = CreateObject( "WScript.Shell" )
Set fso = CreateObject("Scripting.FileSystemObject")

'----------------------------------------------------------------------------------------------------------------------------------------------
'----------------------------------------------------------------------------------------------------------------------------------------------
'----------------------------------------------------------------------------------------------------------------------------------------------
'START
'Custom Actions
'----------------------------------------------------------------------------------------------------------------------------------------------
Dim ProgFol, pos, userName, profile, appdata, userSID, cv, OSVer
Dim config

If bitWise = "32bit" Then
	ProgFol = wso.ExpandEnvironmentStrings("%ProgramFiles%")
ElseIf bitWise = "64bit" Then
	ProgFol = wso.ExpandEnvironmentStrings("%ProgramFiles(x86)%")
End If

userSID = GetCurrUserSID
userName = GetCurrUserName

pos = InStr(userName,"\")
If pos > 0 Then
	userName=Right(userName,Len(userName)-pos)
End If

cv = CompareVersions(DetectOSVer, "6.0")
OSVer = (cv >= 0)

appdata = ""

If OSVer Then
	profile = ReadHKCUValue("\Volatile Environment\USERPROFILE")
	If Left(profile,5) = "##ERR" Then profile = ""
	If Not profile = "" Then appdata = profile & "\AppData\LocalLow\"
Else
	appdata = ReadHKCUValue("\Volatile Environment\APPDATA")
	If Left(appdata,5) = "##ERR" Then appdata = ""
End If

If Not appdata = "" Then
	DeleteFolder appdata & "\Sun\Java\Deployment\security\baseline.versions"
End If

'----------------------------------------------------------------------------------------------------------------------------------------------
Set wso = Nothing
Set fso = Nothing
'----------------------------------------------------------------------------------------------------------------------------------------------
'END
'Custom Actions
'----------------------------------------------------------------------------------------------------------------------------------------------
'----------------------------------------------------------------------------------------------------------------------------------------------
'----------------------------------------------------------------------------------------------------------------------------------------------
'General Functions
'- GetCurrUserSID
'- GetCurrUserName
'- ReadHKCUValue
'- Reg_Read_Str
'- ReadRegStr
'- DetectClientPlatform
'----------------------------------------------------------------------------------------------------------------------------------------------
Sub DeleteFile(FilePath)
	If fso.FileExists(FilePath) Then
		fso.DeleteFile FilePath, True
	End If
End Sub

Sub DeleteFolder(FolderPath)
	On Error Resume Next
	If fso.FolderExists(FolderPath) Then
		fso.DeleteFolder FolderPath, True
	End If
	On Error Goto 0
End Sub

Sub createDirectory(FolderPath)
	If Not fso.FolderExists(FolderPath) Then
		fso.CreateFolder(FolderPath)
	End If
End Sub

Function GetCurrUserSID()
	Dim oReg, arrSubKeys, subkey, arrSubKeys2, subkey2
	GetCurrUserSID = ""
	Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\.\root\default:StdRegProv")
	
	oReg.EnumKey HKEY_USERS,"",arrSubKeys
	
	For Each subkey In arrSubKeys
		oReg.EnumKey HKEY_USERS, subkey, arrSubKeys2
		If Not subkey = ".DEFAULT" Then
			If isArray(arrSubKeys2) Then
				For Each subkey2 In arrSubKeys2
					If subkey2 = "Volatile Environment" Then
						GetCurrUserSID = subkey
						Exit For
					End If
				Next
				If GetCurrUserSID <> "" Then Exit For    
			End If
		End If
	Next
	
	Set oReg = Nothing
End Function

Function GetCurrUserName()
	GetCurrUserName = ReadHKCUValue("\Software\Microsoft\Windows\CurrentVersion\Explorer\Logon User Name")
	If Left(GetCurrUserName,5) = "ERROR" Then GetCurrUserName = ""
End Function

Function ReadHKCUValue(RegPath)
	Dim userSID
	userSID = GetCurrUserSID()
	
	If userSID <> "" Then
		ReadHKCUValue = Reg_Read_Str("HKEY_USERS\" & userSID & RegPath)
	Else
		ReadHKCUValue = "##ERR - couldn't get SID of logged on user."
	End If
End Function

Function Reg_Read_Str(sRegValue)
	Reg_Read_Str = (ReadRegStr(sRegValue, 64))
	'if platform is 64bit and key is not read, read 32bit context key
	If (Reg_Read_Str = "##ERR") And (bitWise = "64bit") Then Reg_Read_Str = ReadRegStr(sRegValue, 32)
End Function

Function ReadRegStr(sRegValue, RegType)
	Dim i, tmpKey, tmpValue, aKeyPathSubSection, hKeyRoot, oCtx, oLocator, oReg, oInParams, oOutParams
	
	aKeyPathSubSection = Split(sRegValue, "\")
	Select Case UCase(aKeyPathSubSection(0))
		Case "HKEY_CLASSES_ROOT", "HKCR"
			hKeyRoot = HKEY_CLASSES_ROOT
		Case "HKEY_CURRENT_USER", "HKCU"
			hKeyRoot = HKEY_CURRENT_USER
		Case "HKEY_LOCAL_MACHINE", "HKLM"
			hKeyRoot = HKEY_LOCAL_MACHINE
		Case "HKEY_USERS", "HKU"
			hKeyRoot = HKEY_USERS
		Case "HKEY_CURRENT_CONFIG"
			hKeyRoot = HKEY_CURRENT_CONFIG
		Case Else
			ReadRegStr = "##ERR"
		Exit Function
	End Select 
	
	tmpKey = ""
	tmpValue = aKeyPathSubSection(UBound(aKeyPathSubSection))
	
	For i = 1 To UBound(aKeyPathSubSection) - 1
		tmpKey = tmpKey & aKeyPathSubSection(i) & "\"
	Next  

    Set oCtx = CreateObject("WbemScripting.SWbemNamedValueSet")
    oCtx.Add "__ProviderArchitecture", RegType

    Set oLocator = CreateObject("Wbemscripting.SWbemLocator")
    Set oReg = oLocator.ConnectServer("", "root\default", "", "", , , , oCtx).Get("StdRegProv")

    Set oInParams = oReg.Methods_("GetStringValue").InParameters
    oInParams.hDefKey = hKeyRoot
    oInParams.sSubKeyName = tmpKey
    oInParams.sValueName = tmpValue

    Set oOutParams = oReg.ExecMethod_("GetStringValue", oInParams, , oCtx)
    If IsNull(oOutParams.sValue) Then 
    	ReadRegStr = "##ERR"
    Else 
    	ReadRegStr = oOutParams.sValue
    End If
    
End Function

Function fKillRegKey(sKeyToDelete, RegType)
Dim sSubKey, sKeyToKill, oInParams, oOutParams, iElement
Dim aKeyPathSubSection, sKeyRoot, hkeyRoot, strKeyRoot, oLocator, oReg, oCtx

aKeyPathSubSection = Split(sKeyToDelete, "\")
Select Case UCase(aKeyPathSubSection(0))
 Case "HKEY_CLASSES_ROOT", "HKCR"
  hKeyRoot = HKEY_CLASSES_ROOT
  strKeyRoot = "HKEY_CLASSES_ROOT"
 Case "HKEY_CURRENT_USER", "HKCU"
  hKeyRoot = HKEY_CURRENT_USER
  strKeyRoot = "HKEY_CURRENT_USER"
 Case "HKEY_LOCAL_MACHINE", "HKLM"
  hKeyRoot = HKEY_LOCAL_MACHINE
  strKeyRoot = "HKEY_LOCAL_MACHINE"
 Case "HKEY_USERS", "HKU"
  hKeyRoot = HKEY_USERS
  strKeyRoot = "HKEY_USERS"
 Case "HKEY_CURRENT_CONFIG"
  hKeyRoot = HKEY_CURRENT_CONFIG
  strKeyRoot = "HKEY_CURRENT_CONFIG"
 Case Else
  fKillRegKey = 1
  Exit Function
End Select 


skeyToKill = ""
For iElement = 1 To UBound(aKeyPathSubSection)
 sKeyToKill = sKeyToKill & "\" & aKeyPathSubSection(iElement)   
Next
If Left(sKeyToKill,1) = "\" Then sKeyToKill = Right(sKeyToKill, Len(sKeyToKill)-1)

Set oCtx = CreateObject("WbemScripting.SWbemNamedValueSet")
oCtx.Add "__ProviderArchitecture", RegType

Set oLocator = CreateObject("Wbemscripting.SWbemLocator")
Set oReg = oLocator.ConnectServer("", "root\default", "", "", , , , oCtx).Get("StdRegProv")

Set oInParams = oReg.Methods_("EnumKey").InParameters
oInParams.hDefKey = hKeyRoot
oInParams.sSubKeyName = sKeyToKill

Set oOutParams = oReg.ExecMethod_("EnumKey", oInParams, , oCtx)

If Not IsNull(oOutParams.sNames) Then
 For Each sSubKey In oOutParams.sNames
	'Recursively call this function, to delete each subkey - and their subkeys, and so on...
  	fKillRegKey strKeyRoot & "\" & sKeyToKill & "\" & sSubKey, RegType
 Next
End If

'if the key has no subkeys, delete it.
Set oInParams = oReg.Methods_("DeleteKey").InParameters
oInParams.hDefKey = hKeyRoot
oInParams.sSubKeyName = sKeyToKill

Set oOutParams = oReg.ExecMethod_("DeleteKey", oInParams, , oCtx)
fKillRegKey = oOutParams.ReturnValue
  
End Function

Function Reg_Hive_Delete(sRegValue)
Dim oRet 
oRet = fKillRegKey(sRegValue,64)

If (oRet <> 0) Then oRet = fKillRegKey(sRegValue,32)
Reg_Hive_Delete = (oRet = 0)

End Function
' -----------------------------------------------------------------------------

Function DetectOSVer()
	Dim objWMIService, colOperatingSystems, objOperatingSystem, oVersion

	Set objWMIService = GetObject("winmgmts:{impersonationLevel=impersonate}!\\.\root\cimv2")
	Set colOperatingSystems = objWMIService.ExecQuery("Select * from Win32_OperatingSystem")
	
	For Each objOperatingSystem In colOperatingSystems
		oVersion = objOperatingSystem.Version
	Next

	DetectOSVer = oVersion
	
End Function

Function CompareVersions(ByVal Version1, ByVal Version2)
	Dim Ver1, Ver2, Result
	Ver1 = GetVersionStringAsArray(Version1)
	Ver2 = GetVersionStringAsArray(Version2)
	
	If Ver1(0) < Ver2(0) Then
		Result = -1
	ElseIf Ver1(0) = Ver2(0) Then
		If Ver1(1) < Ver2(1) Then
			Result = -1
		ElseIf Ver1(1) = Ver2(1) Then
			Result = 0
		Else
			Result = 1
		End If
	Else
		Result = 1
	End If
	
	CompareVersions = Result
End Function

' Bitwise left shift.
Function Lsh(ByVal N, ByVal Bits)
	Lsh = N * (2 ^ Bits)
End Function

Function GetVersionStringAsArray(ByVal Version)
	Dim VersionAll, VersionParts, N, chr
	VersionAll = Array(0, 0, 0, 0)
	
	If InStr(Version, ".") > 0 Then 
		chr = "."
	ElseIf InStr(Version, ",") > 0 Then
		chr = ","
	End If
	
	VersionParts = Split(Version, chr)
	
	For N = 0 To UBound(VersionParts)
		VersionAll(N) = CLng(VersionParts(N))
	Next

	Dim Hi, Lo
	Hi = Lsh(VersionAll(0), 16) + VersionAll(1)
	Lo = Lsh(VersionAll(2), 16) + VersionAll(3)
  
	GetVersionStringAsArray = Array(Hi, Lo)
End Function