#!/usr/bin/env python

import subprocess

def generateReleaseNotes(destinationFilePath):
    print ("Generating {0}...".format(destinationFilePath))

    data = None
    with open(destinationFilePath, "r") as notes: 
        data = notes.read()

    with open(destinationFilePath, "w") as notes:
        p = subprocess.Popen("git describe --tags --abbrev=0", stdout=subprocess.PIPE, shell=True)
        out, err = p.communicate()

        tag = out.decode().rstrip("\n")
        if len(tag) == 0:
            return;

        notes.write("\n========================================================\n")
        p = subprocess.Popen("git log {0}..HEAD --pretty=format:'* [%h] [%as] %s' --reverse".format(tag), stdout=subprocess.PIPE, shell=True)
        out, err = p.communicate()

        for i, line in enumerate(out.decode().rstrip("\n").split("\n")):
            notes.write(line + "\n")

        notes.write(data)


if __name__ == '__main__':
    generateReleaseNotes('changelog.txt')
