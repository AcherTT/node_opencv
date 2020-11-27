cmd_Release/image.node := ln -f "Release/obj.target/image.node" "Release/image.node" 2>/dev/null || (rm -rf "Release/image.node" && cp -af "Release/obj.target/image.node" "Release/image.node")
