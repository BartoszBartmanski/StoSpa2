#!/bin/bash
set -e -x

# Install a system package required by our library
yum install -y atlas-devel cmake3
mkdir ~/bin
export PATH=~/bin:$PATH
ln -s /usr/bin/cmake3 ~/bin/cmake

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install -r /io/dev-requirements.txt
    "${PYBIN}/pip" wheel /io/ -w wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    auditwheel repair "$whl" --plat manylinux2010_x86_64 -w /io/wheelhouse/
done

# Install packages and test
for PYBIN in /opt/python/*/bin/; do
    "${PYBIN}/pip" install pystospa --no-index -f /io/wheelhouse
done
