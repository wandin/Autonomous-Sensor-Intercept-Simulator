$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Resolve-Path "$scriptDir\.."

$uproject = "$projectRoot\SensorIntercept.uproject"

$ueRoot = $env:UE_ROOT

if (-not $ueRoot)
{
    Write-Host "UE_ROOT not set. Skipping Unreal tests in CI."
    exit 0
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