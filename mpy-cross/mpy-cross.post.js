
    MpyCross.compile = (data) => {
        FS.writeFile('main.py', data, { encoding: 'utf-8' });
        const ret = MpyCross._mpy_cross_compile();
        if (ret !== 0) {
            throw Error('Failed to compile');
        }
        return FS.readFile('main.mpy', { encoding: 'binary' });
    }
