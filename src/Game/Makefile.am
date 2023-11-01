# Remove rgl Includes.
# INCLUDES = -I$(top_srcdir)/src/ThirdParty/CRC -I$(top_srcdir)/src/ThirdParty/JPG -I$(top_srcdir)/src/ThirdParty/LZSS -I$(top_srcdir)/src/rgl -I$(top_srcdir)/src/SDL -I$(top_srcdir)/src/Ships -I$(top_srcdir)/src/ThirdParty/Titan
AM_CPPFLAGS = -I$(top_srcdir)/src/ThirdParty/CRC -I$(top_srcdir)/src/ThirdParty/LZSS -I$(top_srcdir)/src/SDL -I$(top_srcdir)/src/Ships -I$(top_srcdir)/src/ThirdParty/Titan

# Task functions rely on the frame pointer, but it doesn't seem that we can
# enable it on a per-function basis with GCC.
# Another option would be to split off the tasks into separate sources and
# compile them without using the frame pointer.

# GCC in Mingw32 defaults to -malign-double
#  Original CFLAGS line
# CFLAGS += -fno-omit-frame-pointer -mno-align-double
#  Additional Modified CFLAGS line
# CFLAGS += -malign-double -Wall -fno-strict-aliasing -Wextra -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls
AM_CFLAGS = -Wall -fno-strict-aliasing -Wextra

noinst_LIBRARIES = libhw_Game.a
libhw_Game_a_SOURCES = AIAttackMan.c AIAttackMan.h AIDefenseMan.c AIDefenseMan.h AIEvents.c AIEvents.h AIFeatures.h AIFleetMan.c AIFleetMan.h AIHandler.c AIHandler.h AIMoves.c AIMoves.h AIOrders.c AIOrders.h AIPlayer.c AIPlayer.h AIResourceMan.c AIResourceMan.h AIShip.c AIShip.h AITeam.c AITeam.h AITrack.c AITrack.h AIUtilities.c AIUtilities.h AIVar.c AIVar.h Alliance.c Alliance.h Animatic.c Animatic.h Attack.c Attack.h Attributes.h AutoDownloadMap.c AutoDownloadMap.h AutoLOD.c AutoLOD.h Battle.c Battle.h BigFile.c BigFile.h Blobs.c Blobs.h BMP.c BMP.h Bounties.c Bounties.h B-Spline.c B-Spline.h BTG.c BTG.h Camera.c CameraCommand.c CameraCommand.h Camera.h Captaincy.c Captaincy.h ChannelFSM.c ChannelFSM.h Chatting.c Chatting.h Clamp.c Clamp.h ClassDefs.h Clipper.c Clipper.h Clouds.c Clouds.h Collision.c Collision.h Color.c Color.h ColPick.c ColPick.h CommandDefs.h CommandLayer.c CommandLayer.h CommandNetwork.c CommandNetwork.h CommandWrap.c CommandWrap.h ConsMgr.c ConsMgr.h cpuid.h Crates.c Crates.h Damage.c Damage.h Debug.c Debug.h Demo.c Demo.h Dock.c Dock.h ETG.c ETG.h Eval.c Eval.h FastMath.h FEColour.h FEFlow.c FEFlow.h FEReg.c FEReg.h File.c File.h FlightMan.c FlightManDefs.h FlightMan.h FontReg.c FontReg.h Formation.c FormationDefs.h Formation.h GameChat.c GameChat.h GamePick.c GamePick.h GameStats.h Globals.c Globals.h Gun.c Gun.h Hash.c Hash.h HorseRace.c HorseRace.h HS.c HS.h InfoOverlay.c InfoOverlay.h KAS.c KASFunc.c KASFunc.h KAS.h KeyBindings.c KeyBindings.h Key.c Key.h KNITransform.c LagPrint.c LagPrint.h LaunchMgr.c LaunchMgr.h LevelLoad.c LevelLoad.h Light.c Light.h LinkedList.c LinkedList.h LOD.c LOD.h MadLinkIn.c MadLinkInDefs.h MadLinkIn.h Matrix.c Matrix.h MaxMultiplayer.h Memory.c Memory.h MeshAnim.c MeshAnim.h Mesh.c Mesh.h MEX.c MEX.h MultiplayerGame.c MultiplayerGame.h MultiplayerLANGame.c MultiplayerLANGame.h NavLights.c NavLights.h Nebulae.c Nebulae.h NetCheck.c NetCheck.h NIS.c NIS.h Objectives.c Objectives.h ObjTypes.c ObjTypes.h Options.c Options.h Particle.c Particle.h Physics.c Physics.h PiePlate.c PiePlate.h Ping.c Ping.h PlugScreen.c PlugScreen.h ProfileTimers.c ProfileTimers.h RaceDefs.h Randy.c Randy.h Region.c Region.h ResCollect.c ResCollect.h ResearchAPI.c ResearchAPI.h ResearchGUI.c ResearchGUI.h SaveGame.c SaveGame.h ScenPick.c ScenPick.h Scroller.c Scroller.h Select.c Select.h Sensors.c Sensors.h Shader.c Shader.h ShipSelect.c ShipSelect.h ShipView.c ShipView.h SinglePlayer.c SinglePlayer.h SoundEvent.c SoundEventDefs.h SoundEvent.h SoundEventPlay.c SoundEventPrivate.h SoundEventStop.c SoundMusic.h SoundStructs.h SpaceObj.h SpeechEvent.c SpeechEvent.h Star3d.c Star3d.h Stats.c StatScript.c StatScript.h Stats.h StringSupport.c StringSupport.h StringsOnly.h Subtitle.c Subtitle.h Switches.h Tactical.c Tactical.h Tactics.c Tactics.h TaskBar.c TaskBar.h Task.c Task.h Teams.c Teams.h Timer.c Timer.h TitanNet.c TitanNet.h Tracking.c Tracking.h TradeMgr.c TradeMgr.h Trails.c Trails.h Transformer.c Transformer.h Tutor.c Tutor.h Tweak.c Tweak.h Twiddle.c Twiddle.h Types.c Types.h UIControls.c UIControls.h Undo.c Undo.h Universe.c Universe.h UnivUpdate.c UnivUpdate.h Vector.c Vector.h VolTweakDefs.h Volume.c Volume.h wrapped_functions.h

# KNITransform.c requires SSE instructions, but we don't want to force SSE
# instructions throughout the project.
if X86_CPU
KNITransform.o: KNITransform.c
	if $(COMPILE) -msse -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" \
	  -c -o $@ `test -f '$<' || echo '$(srcdir)/'`$<; \
	then mv "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; \
	else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; \
	fi
else
KNITransform.o: StubTransform.c
	if $(COMPILE) -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" \
	  -c -o $@ `test -f '$<' || echo '$(srcdir)/'`$<; \
	then mv "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; \
	else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; \
	fi
endif
# Optimization thrashes Task.c, ETG.c, and FastMath.c (although we should
# definitely fix FastMath.c at some point, being that it is straight C).
Task.o: Task.c
	if $(COMPILE) -O0 -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" \
	  -c -o $@ `test -f '$<' || echo '$(srcdir)/'`$<; \
	then mv "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; \
	else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; \
	fi

ETG.o: ETG.c wrapped_functions.h
	if $(COMPILE) -O0 -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" \
	  -c -o $@ `test -f '$<' || echo '$(srcdir)/'`$<; \
	then mv "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; \
	else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; \
	fi

FastMath.o: FastMath.c
	if $(COMPILE) -O0 -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" \
	  -c -o $@ `test -f '$<' || echo '$(srcdir)/'`$<; \
	then mv "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; \
	else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; \
	fi

wrapped_functions.h: functions.h wrap.h wrap_types.h
	if $(COMPILE) -O0 -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" \
	  -E -o $@ `test -f '$<' || echo '$(srcdir)/'`$<; \
	then mv "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; \
	else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; \
	fi
