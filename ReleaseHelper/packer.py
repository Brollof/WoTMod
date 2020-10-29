#!/usr/bin/env python

import os
import re
import zipfile
import subprocess


def get_fullpath(filename):
    script_dir = os.path.dirname(os.path.realpath(__file__))
    return os.path.join(script_dir, filename)

BUILDER_PATH = r'D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv'
PROJECT_ROOT = get_fullpath(r"..\WoTbyte")
ARCHIVE_SUBDIR = "WoTbyte"
SOLUTION_PATH = os.path.join(PROJECT_ROOT, "WoTbyte.sln")
VERSION_FILE = os.path.join(PROJECT_ROOT, "inc", "Version.h")
FILES_TO_PACK = [
    os.path.join(PROJECT_ROOT, "bin", "Release", "WoTbyte.exe"),
    os.path.join(PROJECT_ROOT, "libs", "7zip", "7z.dll"),
    os.path.join(PROJECT_ROOT, "libs", "curl", "bin", "libcurl.dll"),
    os.path.join(PROJECT_ROOT, "settings.ini")
]


def get_archive_path(file):
    # example:
    #   file: a/b/c/file.txt
    #   returns: subdir/file.txt
    return os.path.join(ARCHIVE_SUBDIR, os.path.basename(file))


def generate_archive_name():
    try:
        with open(VERSION_FILE, "r") as file:
            data = file.readlines()
    except Exception as ex:
        print(ex)
        return None

    version = []
    for line in data:
        # this regex requires specific order of versions
        if res := re.search(r"(VERSION_MAJOR|VERSION_MINOR|VERSION_REVISION)\s+(\d+)", line):
            version.append(res[2])

    return f"{ARCHIVE_SUBDIR.lower()}-" + "v{}.{}.{}.zip".format(*version)


def main():
    try:
        proc = subprocess.run([BUILDER_PATH, SOLUTION_PATH, '/Rebuild', 'Release'], shell=True)
    except Exception as ex:
        print(ex)

    zip_name = generate_archive_name()
    if not zip_name:
        print("Empty zip name :(")
        return

    try:
        with zipfile.ZipFile(zip_name, 'w') as myzip:
            for file in FILES_TO_PACK:
                myzip.write(file, get_archive_path(file))
    except Exception as ex:
        print(ex)
        return

    print(f"Zip created: {zip_name}")


if __name__ == '__main__':
    main()