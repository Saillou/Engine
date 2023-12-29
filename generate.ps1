 param (
    [switch] $Cleanup,   # Clean generated files
    [switch] $NoBuild,   # Don't build binaries
    [switch] $NoInstall  # Don't install resources
 )

function clearPrevious() 
{
    Write-Host "Deleting ./_solution." -ForegroundColor DarkRed 
    if (Test-Path -Path "./_solution")
    {
        Remove-Item -Path "./_solution" -Recurse -Force
    }

    Write-Host "Deleting ./_output." -ForegroundColor DarkRed 
    if (Test-Path -Path "./_output")
    {
        Remove-Item -Path "./_output" -Recurse -Force
    }
}

function generateSolution() 
{
    Write-Host "Creating ./_solution." -ForegroundColor DarkGreen 
    if (-Not(Test-Path -Path "./_solution")) 
    {
        New-Item -ItemType Directory -Path "./_solution"
    }

    Push-Location -Path  "./_solution"

    Write-Host "Generating Visual Studio solution." -ForegroundColor DarkGreen 
    cmake .. -G "Visual Studio 17 2022" -A x64

    Pop-Location
}

function compileProject()
{
    Push-Location -Path  "./_solution"

    # Compile for Debug
    Write-Host "Compile for Debug." -ForegroundColor DarkYellow 
    cmake --build . --config Debug

    # Compile for Release
    Write-Host "Compile for Release." -ForegroundColor DarkYellow 
    cmake --build . --config Release

    Pop-Location
}

function installResources()
{
    Write-Host "Installing resources." -ForegroundColor DarkMagenta 

    # Install for Debug
    Copy-Item -Path "$PSScriptRoot/Resources" -Destination "$PSScriptRoot/_output/bin/debug/Resources" -Recurse -Force
    Copy-Item -Path "$PSScriptRoot/Dlls/*" -Destination "$PSScriptRoot/_output/bin/debug" -Recurse -Force

    # Install for Release
    Copy-Item -Path "$PSScriptRoot/Resources" -Destination "$PSScriptRoot/_output/bin/release/Resources" -Recurse -Force
    Copy-Item -Path "$PSScriptRoot/Dlls/*" -Destination "$PSScriptRoot/_output/bin/release" -Recurse -Force
}

# Entry point
if($Cleanup) 
{
    clearPrevious
}
else 
{
    Write-Host "Start project generation ..." -ForegroundColor DarkMagenta 

    clearPrevious
    generateSolution

    if(-Not($NoBuild)) 
    {
        compileProject
    }

    if(-Not($NoInstall)) 
    {
        installResources
    }

    Write-Host "... Done." -ForegroundColor DarkMagenta     
}