#!/bin/bash

# Получение пути к текущей директории
currentDir="$(cd "$(dirname "$0")"; pwd)"

# Удаление папок Binaries, Intermediate и Saved из корневой папки
if [ -d "$currentDir/Binaries" ]; then
    echo "Deleting folder: $currentDir/Binaries"
    rm -rf "$currentDir/Binaries"
fi

if [ -d "$currentDir/Intermediate" ]; then
    echo "Deleting folder: $currentDir/Intermediate"
    rm -rf "$currentDir/Intermediate"
fi

if [ -d "$currentDir/Saved" ]; then
    echo "Deleting folder: $currentDir/Saved"
    rm -rf "$currentDir/Saved"
fi

# Удаление файла с расширением .sln из корневой папки
slnFiles=$(find "$currentDir" -maxdepth 1 -name "*.sln")
if [ -n "$slnFiles" ]; then
    echo "Deleting .sln files in: $currentDir"
    rm -f "$currentDir"/*.sln
fi

# Удаление папок Binaries и Intermediate в каждой подпапке внутри Plugins
for pluginDir in "$currentDir/Plugins/"*/; do
    if [ -d "$pluginDir/Binaries" ]; then
        echo "Deleting folder: $pluginDir/Binaries"
        rm -rf "$pluginDir/Binaries"
    fi
    if [ -d "$pluginDir/Intermediate" ]; then
        echo "Deleting folder: $pluginDir/Intermediate"
        rm -rf "$pluginDir/Intermediate"
    fi
done

echo "Complete"

