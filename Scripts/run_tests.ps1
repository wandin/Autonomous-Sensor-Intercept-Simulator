$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Resolve-Path "$scriptDir\.."

$uproject = "$projectRoot\SensorIntercept.uproject"

# Try environment variable first
$ueRoot = $env:UE_ROOT

if (-not $ueRoot)
{
    Write-Host "UE_ROOT environment variable not set."
    Write-Host "Example:"
    Write-Host '$env:UE_ROOT="C:\Program Files\Epic Games\UE_5.3"'
    exit 1
}

$ue = Join-Path $ueRoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$ubt = Join-Path $ueRoot "Engine\Build\BatchFiles\Build.bat"

Write-Host "Using Unreal Engine at: $ueRoot"

Write-Host "Building project..."

& "$ubt" SensorInterceptEditor Win64 Development "$uproject" -waitmutex

if ($LASTEXITCODE -ne 0)
{
    Write-Error "Build failed"
    exit 1
}

Write-Host "Running Automation Tests..."

& "$ue" "$uproject" `
    -ExecCmds="Automation RunTests SensorIntercept;Quit" `
    -unattended `
    -nop4 `
    -stdout `
    -FullStdOutLogOutput

if ($LASTEXITCODE -ne 0)
{
    Write-Error "Tests failed"
    exit 1
}

Write-Host "All tests passed."