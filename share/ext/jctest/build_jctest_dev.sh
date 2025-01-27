#!/usr/bin/env bash
# Copyright 2020-2023 The Defold Foundation
# Copyright 2014-2020 King
# Copyright 2009-2014 Ragnar Svensson, Christian Murray
# Licensed under the Defold License version 1.0 (the "License"); you may not use
# this file except in compliance with the License.
# 
# You may obtain a copy of the License, together with FAQs at
# https://www.defold.com/license
# 
# Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied. See the License for the
# specific language governing permissions and limitations under the License.



BRANCH=message-flush
URL=https://github.com/JCash/jctest/archive/refs/heads/${BRANCH}.zip
PREFIX=./tmp

wget -q $URL

rm -rf ./tmp_unpacked
unzip -q ${BRANCH}.zip -d tmp_unpacked

mkdir -p ${PREFIX}/include/jc_test

cp -v ./tmp_unpacked/jctest-${BRANCH}/src/jc_test.h ${PREFIX}/include/jc_test

cd ${PREFIX} && tar czvf ../jctest-dev-common.tar.gz include

rm -rf ./tmp_unpacked
rm -rf ./tmp