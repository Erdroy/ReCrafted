#!/bin/bash
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
cd SCRIPTPATH
dotnet ./Tools/ReCrafted.Tools.ProjectManager.dll -c "$@"