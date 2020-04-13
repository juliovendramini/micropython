/// <reference types="emscripten" />
declare module '@pybricks/mpy-cross' {
    interface MpyCross extends EmscriptenModule {
        /**
         * Compiles 'main.py' to 'main.mpy'
         */
        compile(fileContents: string): Uint8Array;
    }
    var MpyCross: MpyCross;
    export default MpyCross;
}
