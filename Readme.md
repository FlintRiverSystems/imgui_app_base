

# WASM build

Must be built on linux or wsl...

Install emsdk:

   ```
   cd $FRSTPSRC
   git clone https://github.com/emscripten-core/emsdk
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   ```

Run ``./build_wasm.sh`` in the app_example folder
	
Run ``./run_server.sh``

Open http://localhost:8000 in chrome


