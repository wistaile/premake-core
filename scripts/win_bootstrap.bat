@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

SET	VsTargetVersion="%1"
IF "%1" == "" (
    SET	VsTargetVersion="vs2015"
)

CALL :SetupVisualStudioEnvironment %VsTargetVersion%

IF %ERRORLEVEL% NEQ 0 (
    ECHO Failed to setup Visual Studio environment for version %VsTargetVersion%
    EXIT /b %ERRORLEVEL%
)

@ECHO ON

cd "%~dp0\.."

nmake MSDEV=%VsTargetVersion% -f Bootstrap.mak windows

@ECHO OFF

EXIT /b %ERRORLEVEL%

REM ===========================================================================

REM Utils

REM ===========================================================================

REM %1: PremakeVsVersion -> ex: vs2010
:SetupVisualStudioEnvironment
SET "PremakeVsVersion=%1"

CALL :TranslateVisualVersionFromPremake %PremakeVsVersion% VsVersion

IF %ERRORLEVEL% NEQ 0 ( EXIT /b %ERRORLEVEL% )

REM ref: https://github.com/Microsoft/vswhere/wiki/Start-Developer-Command-Prompt

SET VsWherePath="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

IF EXIST %VsWherePath% (
    SET "VsWhereCmdLine=!VsWherePath! -nologo -legacy -latest -version [%VsVersion%.0,%VsVersion%.99] -property installationPath"
    FOR /F "usebackq delims=" %%i in (`!VsWhereCmdLine!`) DO (
        IF EXIST "%%i\Common7\Tools\vsdevcmd.bat" (
            CALL "%%i\Common7\Tools\vsdevcmd.bat"
            EXIT /b
        )
    )
)
REM Legacy

SET "VsVersion_NoPoint=%VsVersion:.=%"
SET "VsEnvVar=VS%VsVersion_NoPoint%COMNTOOLS"
SET "VsPath=!%VsEnvVar%!"

IF EXIST "%VsPath%vsdevcmd.bat" (
    CALL "%VsPath%vsdevcmd.bat"
    EXIT /b
)

REM Instance or command prompt not found
EXIT /b 2
REM :SetupVisualStudioEnvironment

REM ===========================================================================

REM %1: Premake-style VSversion -> ex: vs2015
REM %2 (output): VisualStudio-style VSversion -> ex: 14.0
:TranslateVisualVersionFromPremake
IF %1 == "vs2010" (
    SET "%2=10"
    EXIT /b
)
IF %1 == "vs2012" (
    SET "%2=11"
    EXIT /b
)
IF %1 == "vs2013" (
    SET "%2=12"
    EXIT /b
)
IF %1 == "vs2015" (
    SET "%2=14"
    EXIT /b
)
IF %1 == "vs2017" (
    SET "%2=15"
    EXIT /b
)
echo Failed to translate VisualStudio version
EXIT /b 2
REM :GetVsVarsPathFromVsWhere

ENDLOCAL
