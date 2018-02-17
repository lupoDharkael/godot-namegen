git clone https://github.com/GodotNativeTools/cpp_bindings
git clone https://github.com/GodotNativeTools/godot_headers
cd cpp_bindings
scons godotbinpath="godot" headers="../godot_headers/" p=linux generate_bindings=yes
cd ..
scons
