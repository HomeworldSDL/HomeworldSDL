#define get_arg(n) do_get_arg(effect, stat, opcode, n)

void partSetTrHandle(trhandle t, udword u0, udword v0, udword u1, udword v1);

memsize wrap_partSetTrHandle(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    partSetTrHandle(
        get_arg(0),
        get_arg(1),
        get_arg(2),
        get_arg(3),
        get_arg(4));

    return 0;
}

void partModifyTexture(psysPtr psys, trhandle tex, udword u0, udword v0, udword u1, udword v1);

memsize wrap_partModifyTexture(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    udword var0 = do_get_arg(effect, stat, opcode, 0);
    udword var1 = do_get_arg(effect, stat, opcode, 1);
    udword var2 = do_get_arg(effect, stat, opcode, 2);
    udword var3 = do_get_arg(effect, stat, opcode, 3);
    udword var4 = do_get_arg(effect, stat, opcode, 4);
    udword var5 = do_get_arg(effect, stat, opcode, 5);
    memsize (*func_ptr)(udword, udword, udword, udword, udword, udword) = (memsize(*)(udword, udword, udword, udword, udword, udword))opcode->function;

    return func_ptr(var0, var1, var2, var3, var4, var5);
}

void partSetMeshdata(meshdata *m);

memsize wrap_partSetMeshdata(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    udword var0 = do_get_arg(effect, stat, opcode, 0);
    memsize (*func_ptr)(udword) = (memsize(*)(udword))opcode->function;

    return func_ptr(var0);
}

void etgCreateCallbackSetup(Effect *effect, sdword codeOffset);

memsize wrap_etgCreateCallbackSetup(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    udword var0 = do_get_arg(effect, stat, opcode, 0);
    memsize (*func_ptr)(Effect *, udword) = (memsize(*)(Effect *, udword))opcode->function;

    return func_ptr(effect, var0);
}

memsize etgSpawnNewEffect(Effect *effect, etgeffectstatic *stat, sdword nParams, ...);

udword wrap_etgSpawnNewEffect(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    udword nParams = opcode->nParameters;
    memsize intParam[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (udword i = 0; i < nParams; i++)
    {
        intParam[i] = get_arg(i);
    }

    etgSpawnNewEffect(
        effect,
        (etgeffectstatic *)intParam[0],
        (sdword)intParam[1], // nParams
        intParam[2],
        intParam[3],
        intParam[4],
        intParam[5],
        intParam[6],
        intParam[7]);

    return (etgFunctionSize(nParams));
}

void etgCreateEffects(Effect *effect, etgeffectstatic *stat, sdword number, sdword dist, sdword nParams, ...);

udword wrap_etgCreateEffects(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    udword nParams = opcode->nParameters;
    memsize intParam[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (udword i = 0; i < nParams; i++)
    {
        intParam[i] = get_arg(i);
    }

    etgCreateEffects(
        effect,
        (etgeffectstatic *)intParam[0],
        (sdword)intParam[1],
        (sdword)intParam[2],
        (sdword)intParam[3], // nParams
        intParam[4],
        intParam[5],
        intParam[6],
        intParam[7]);

    return (etgFunctionSize(nParams));
}
