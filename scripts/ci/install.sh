#!/usr/bin/env bash
set -e

echo `pwd`

pip install pathlib
python scripts/ci/common/install_required_addons.py

ls -la ~/openFrameworks/addons
