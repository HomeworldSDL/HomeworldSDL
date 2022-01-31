#!/usr/bin/env python

import sys
import os
from subprocess import call, DEVNULL

in_dir = os.path.dirname(__file__)
biggie_inputs = []

for root, dirs, files in os.walk(in_dir):
	for file in files:
		if file in ("README.txt", "meson.build", os.path.basename(__file__)):
			continue

		path = os.path.abspath(os.path.join(root, file))
		path = os.path.relpath(path, in_dir)

		if "/." in path:
			continue

		biggie_inputs.append(path)

exit(call((sys.argv[1], "-f", os.path.abspath(sys.argv[2]), *biggie_inputs), cwd = in_dir, stdout = DEVNULL))
