#!/bin/bash

set -e

source /opt/ros/humble/setup.bash

if [ -f /opt/uros_ws/install/setup.bash ]; then
    source /opt/uros_ws/install/setup.bash
fi

if [ -f /workspace/install/setup.bash ]; then
    source /workspace/install/setup.bash
fi

exec "$@"