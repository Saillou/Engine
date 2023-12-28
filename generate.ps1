function clearPrevious() 
{
    # Delete temporary folder
    if (Test-Path -Path "./_solution")
    {
        Remove-Item -Path "./_solution" -Recurse -Force
    }

    # Delete previous build
    if (Test-Path -Path "./_output")
    {
        Remove-Item -Path "./_output" -Recurse -Force
    }
}

function generateSolution() 
{
    # Create temporary folder
    if (-Not(Test-Path -Path "./_solution")) 
    {
        New-Item -ItemType Directory -Path "./_solution"
    }

    Push-Location -Path  "./_solution"

    # Generate solution
    cmake .. -G "Visual Studio 17 2022" -A x64

    Pop-Location
}

function compileProject()
{
    Push-Location -Path  "./_solution"

    # Compile for Debug
    cmake --build . --config Debug

    # Compile for Release
    cmake --build . --config Release

    Pop-Location
}

function installResources()
{
    # Install for Debug
    Copy-Item -Path "$PSScriptRoot/Resources" -Destination "$PSScriptRoot/_output/bin/debug/Resources" -Recurse -Force

    # Install for Release
    Copy-Item -Path "$PSScriptRoot/Resources" -Destination "$PSScriptRoot/_output/bin/release/Resources" -Recurse -Force
}

# Entry point
clearPrevious
generateSolution
compileProject
installResources