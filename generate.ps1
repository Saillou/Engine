 param (
    [switch] $Cleanup,   # Clean generated files
    [switch] $NoBuild,   # Don't build binaries
    [switch] $NoInstall, # Don't install resources
    [switch] $NoCleanup, # Don't clean
    [switch] $VS2019     # Use Visual Studio 2019 instead of 2022
 )

function clearPrevious() 
{
    Write-Host "Deleting ./_solution." -ForegroundColor DarkRed 
    if (Test-Path -Path "$PSScriptRoot/_solution")
    {
        Remove-Item -Path "$PSScriptRoot/_solution" -Recurse -Force
    }

    Write-Host "Deleting ./_output." -ForegroundColor DarkRed 
    if (Test-Path -Path "$PSScriptRoot/_output")
    {
        Remove-Item -Path "$PSScriptRoot/_output" -Recurse -Force
    }

    Write-Host "Deleting ./imgui.ini" -ForegroundColor DarkRed 
    if (Test-Path -Path "$PSScriptRoot/imgui.ini")
    {
        Remove-Item -Path "$PSScriptRoot/imgui.ini" -Recurse -Force
    }
}

function generateSolution() 
{
    Write-Host "Creating ./_solution." -ForegroundColor DarkGreen 
    if (-Not(Test-Path -Path "$PSScriptRoot/_solution")) 
    {
        New-Item -ItemType Directory -Path "$PSScriptRoot/_solution"
    }

    Push-Location -Path  "$PSScriptRoot/_solution"

    Write-Host "Generating Visual Studio solution." -ForegroundColor DarkGreen
    if($VS2019)
    {
        cmake .. -G "Visual Studio 16 2019" -A x64
    } 
    else
    {
        cmake .. -G "Visual Studio 17 2022" -A x64
    }
    

    Pop-Location
}

function compileProject()
{
    Push-Location -Path  "$PSScriptRoot/_solution"

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
    Copy-Item -Path "$PSScriptRoot/Resources" -Destination "$PSScriptRoot/_output/bin/debug/Resources"   -Recurse -Force
    Copy-Item -Path "$PSScriptRoot/Dlls/*"    -Destination "$PSScriptRoot/_output/bin/debug"             -Recurse -Force

    # Install for Release
    Copy-Item -Path "$PSScriptRoot/Resources" -Destination "$PSScriptRoot/_output/bin/release/Resources" -Recurse -Force
    Copy-Item -Path "$PSScriptRoot/Dlls/*"    -Destination "$PSScriptRoot/_output/bin/release"           -Recurse -Force
}

# Entry point
if($Cleanup) 
{
    clearPrevious
}
else 
{
    Write-Host "Start project generation ..." -ForegroundColor DarkMagenta 

    if(-Not($NoCleanup))
    {
        clearPrevious
    }
    
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