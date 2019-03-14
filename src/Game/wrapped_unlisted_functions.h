#define get_arg(n) do_get_arg(effect, stat, opcode, n)

void partSetTrHandle(trhandle t, udword u0, udword v0, udword u1, udword v1);

memsize wrap_partSetTrHandle(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    partSetTrHandle(
        (trhandle)get_arg(0),
        (udword)get_arg(1),
        (udword)get_arg(2),
        (udword)get_arg(3),
        (udword)get_arg(4));

    return 0;
}

void partModifyTexture(psysPtr psys, trhandle tex, udword u0, udword v0, udword u1, udword v1);

memsize wrap_partModifyTexture(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    partModifyTexture(
        (psysPtr)get_arg(0),
        (trhandle)get_arg(1),
        (udword)get_arg(2),
        (udword)get_arg(3),
        (udword)get_arg(4),
        (udword)get_arg(5));

    return 0;
}

void partSetMeshdata(meshdata *m);

memsize wrap_partSetMeshdata(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    partSetMeshdata((meshdata *)get_arg(0));

    return 0;
}

void etgCreateCallbackSetup(Effect *effect, sdword codeOffset);

memsize wrap_etgCreateCallbackSetup(Effect *effect, struct etgeffectstatic *stat, etgfunctioncall *opcode)
{
    etgCreateCallbackSetup(effect, (sdword)get_arg(0));

    return 0;
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
