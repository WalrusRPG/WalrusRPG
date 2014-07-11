#!/usr/bin/env python3

import sys, getopt, os
from PIL import Image

def getvalue(i, im):
    r, g, b = im[i]
    color = (int(r * 31 / 255) << 11) + (int(g * 63 / 255) << 5) + (int(b * 31 / 255))
    return "0x" + format(color, "04X")

file = sys.argv[1]

im = Image.open(file)
width, height = im.size
im = list(im.convert("RGB").getdata())
sprite = "unsigned short " + os.path.splitext(file)[0] + "[] =\n{\n"
sprite += "\t" + str(width) + ", " + str(height) + ", "
sprite += getvalue(0, im)
sprite += ",\n\t"
for i in range(width * height):
    sprite += getvalue(i, im)
    if i != width * height - 1:
        if (i + 1) % 10 == 0:
            sprite += ",\n\t"
        else:
            sprite += ", "
sprite += "\n};"

print(sprite)

