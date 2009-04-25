#define get_arg(n) do_get_arg(effect, stat, opcode, n)

#define memsize udword

void partSetTrHandle(trhandle t, udword u0, udword v0, udword u1, udword v1);

udword wrap_partSetTrHandle(Effect *effect, struct etgeffectstatic *stat ,etgfunctioncall *opcode)
{
	udword var0 = do_get_arg(effect, stat, opcode, 0);
	udword var1 = do_get_arg(effect, stat, opcode, 1);
	udword var2 = do_get_arg(effect, stat, opcode, 2);
	udword var3 = do_get_arg(effect, stat, opcode, 3);
	udword var4 = do_get_arg(effect, stat, opcode, 4);
	udword (*func_ptr)(udword, udword, udword, udword, udword) = (udword (*)(udword, udword, udword, udword, udword)) opcode->function;

	return func_ptr(var0, var1, var2, var3, var4);
}


void partModifyTexture(psysPtr psys, trhandle tex, udword u0, udword v0, udword u1, udword v1);

udword wrap_partModifyTexture(Effect *effect, struct etgeffectstatic *stat ,etgfunctioncall *opcode)
{
	udword var0 = do_get_arg(effect, stat, opcode, 0);
	udword var1 = do_get_arg(effect, stat, opcode, 1);
	udword var2 = do_get_arg(effect, stat, opcode, 2);
	udword var3 = do_get_arg(effect, stat, opcode, 3);
	udword var4 = do_get_arg(effect, stat, opcode, 4);
	udword var5 = do_get_arg(effect, stat, opcode, 5);
	udword (*func_ptr)(udword, udword, udword, udword, udword, udword) = (udword (*)(udword, udword, udword, udword, udword, udword)) opcode->function;

	return func_ptr(var0, var1, var2, var3, var4, var5);
}

void partSetMeshdata(meshdata *m);

udword wrap_partSetMeshdata(Effect *effect, struct etgeffectstatic *stat ,etgfunctioncall *opcode)
{
	udword var0 = do_get_arg(effect, stat, opcode, 0);
	udword (*func_ptr)(udword) = (udword (*)(udword)) opcode->function;

	return func_ptr(var0);
}

void etgCreateCallbackSetup(Effect *effect, sdword codeOffset);

udword wrap_etgCreateCallbackSetup(Effect *effect, struct etgeffectstatic *stat ,etgfunctioncall *opcode)
{
	udword var0 = do_get_arg(effect, stat, opcode, 0);
	udword (*func_ptr)(Effect *, udword) = (udword (*)(Effect *, udword)) opcode->function;

	return func_ptr(effect, var0);
}


memsize etgSpawnNewEffect(Effect *effect, etgeffectstatic *stat, sdword nParams, ...);

udword wrap_etgSpawnNewEffect(Effect *effect, struct etgeffectstatic *stat ,etgfunctioncall *opcode)
{
	udword nParams     = opcode->nParameters;

	memsize param       = 0;
	udword intParam[8] = {0,0,0,0,0,0,0,0};
//	memsize floatParam[8] = {0,0,0,0,0,0,0,0};

	udword i;

	for (i=0;i<opcode->nParameters;i++)
	{
		intParam[i] = get_arg(i);
	}

//        dbgMessagef("Function: %lx:%d   -> %s",((etgfunctioncall *)opcode)->function, nParams+((etgfunctioncall *)opcode)->passThis ,  stat->name);

        switch  (nParams+((etgfunctioncall *)opcode)->passThis){
            case 4: 
//                dbgMessagef("etgSpawnNewEffect %lx,%lx,%lx,%lx", effect,intParam[1],1,intParam[3]);
                etgSpawnNewEffect(effect, intParam[0],1,intParam[2]);
                return (etgFunctionSize(nParams));
                break;
            case 5: 
//                dbgMessagef("etgSpawnNewEffect %lx,%lx,%lx,%lx,%lx", effect,intParam[1],2,intParam[3],intParam[4]);
                etgSpawnNewEffect(effect, intParam[0],2,intParam[2],intParam[3]);
                return (etgFunctionSize(nParams));
                break;
            case 6: 
//                dbgMessagef("etgSpawnNewEffect %lx,%lx,%lx,%lx,%lx,%lx", effect,intParam[1],3,intParam[3],intParam[4],intParam[5]);
                etgSpawnNewEffect(effect, intParam[0],3,intParam[2],intParam[3],intParam[4]);
                return (etgFunctionSize(nParams));
                break;

            case 7:
            case 8:
            case 9:
                return (etgFunctionSize(nParams));
                break;

            default: 
//                dbgMessagef("etgSpawnNewEffect %lx,%lx", effect,intParam[1],intParam[2]);
                break;
        }
	return(etgFunctionSize(nParams));
}


void etgCreateEffects(Effect *effect, etgeffectstatic *stat, sdword number, sdword dist, sdword nParams, ...);

udword wrap_etgCreateEffects(Effect *effect, struct etgeffectstatic *stat ,etgfunctioncall *opcode)
{
	udword nParams     = opcode->nParameters;

	memsize param       = 0;
	udword intParam[8] = {0,0,0,0,0,0,0,0};

	udword i;

	for (i=0;i<opcode->nParameters;i++)
	{
		intParam[i] = get_arg(i);
	}

        switch  (nParams+((etgfunctioncall *)opcode)->passThis){
            case 6: 
//                dbgMessagef("etgCreateEffects %lx,%lx,%lx,%lx,%lx,%lx", effect,intParam[1],intParam[2],intParam[3],1,intParam[5]);
                etgCreateEffects(effect, intParam[0],intParam[1],intParam[2],1, intParam[3]);
                return (etgFunctionSize(nParams));
                break;
            case 7: 
//                dbgMessagef("etgCreateEffects %lx,%lx,%lx,%lx,%lx,%lx,%lx", effect,intParam[1],intParam[2],intParam[3],2,intParam[5],intParam[6]);
                etgCreateEffects(effect, intParam[0],intParam[1],intParam[2],2, intParam[4], intParam[5]);
                return (etgFunctionSize(nParams));
                break;
            case 8: 
//                dbgMessagef("etgCreateEffects %lx,%lx,%lx,%lx,%lx,%lx,%lx,%lx", effect,intParam[1],intParam[2],intParam[3],3,intParam[5],intParam[6],intParam[7]);
                etgCreateEffects(effect, intParam[0],intParam[1],intParam[2],3, intParam[4], intParam[5], intParam[6]);
                return (etgFunctionSize(nParams));
                break;

            case 9:
            case 10:
            case 11:
                return (etgFunctionSize(nParams));
                break;

            default: 
//                dbgMessagef("etgCreateEffects %lx,%lx,%lx,%lx,%lx", effect,intParam[1],intParam[2],intParam[3],intParam[4]);
                break;
        }
	return(etgFunctionSize(nParams));
}


