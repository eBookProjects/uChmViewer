#!/bin/bash

include-what-you-use $* 2>&1 | tee $(basename -- "${!#}").log
exit 0
