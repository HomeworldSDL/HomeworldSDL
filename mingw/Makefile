GIT_VERSION = `git config --get remote.origin.url | sed -r 's/.*(\@|\/\/)(.*)(\:|\/)([^:\/]*)\/([^\/\.]*)\.git/https:\/\/\2\/\4\/\5/'` `git describe --tags --always --dirty=-dev`

BUILDDIR = build/build
BINDIR = bin

JPG_MODULES = interfce.c \
              jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c \
              jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c \
              jcomapi.c jcparam.c jcprepct.c jcsample.c jctrans.c jdapimin.c \
              jdapistd.c jdarith.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c \
              jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c \
              jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c \
              jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c jquant1.c \
              jquant2.c jutils.c jmemmgr.c jmemnobs.c

GAME_MODULES = AIAttackMan.c AIDefenseMan.c AIEvents.c AIFleetMan.c AIHandler.c AIMoves.c AIOrders.c AIPlayer.c AIResourceMan.c AIShip.c AITeam.c AITrack.c AIUtilities.c AIVar.c \
               Alliance.c Animatic.c Attack.c AutoDownloadMap.c AutoLOD.c B-Spline.c Battle.c BigFile.c Blobs.c BMP.c Bounties.c BTG.c Camera.c CameraCommand.c Captaincy.c \
               ChannelFSM.c Chatting.c Clamp.c Clipper.c Clouds.c Collision.c Color.c ColPick.c CommandLayer.c CommandNetwork.c CommandWrap.c ConsMgr.c Crates.c Damage.c Debug.c \
               Demo.c Dock.c ETG.c Eval.c FEFlow.c FEReg.c File.c FlightMan.c FontReg.c Formation.c GameChat.c GamePick.c Globals.c Gun.c Hash.c HorseRace.c HS.c InfoOverlay.c \
               KAS.c KASFunc.c Key.c KeyBindings.c LagPrint.c LaunchMgr.c LevelLoad.c Light.c LinkedList.c LOD.c MadLinkIn.c Matrix.c Memory.c Mesh.c MeshAnim.c MEX.c \
               MultiplayerGame.c MultiplayerLANGame.c NavLights.c Nebulae.c NetCheck.c NIS.c Objectives.c ObjTypes.c Options.c Particle.c Physics.c PiePlate.c Ping.c \
               PlugScreen.c ProfileTimers.c Randy.c Region.c ResCollect.c ResearchAPI.c ResearchGUI.c SaveGame.c ScenPick.c Scroller.c Select.c Sensors.c Shader.c ShipSelect.c \
               ShipView.c SinglePlayer.c SoundEvent.c SoundEventPlay.c SoundEventStop.c SpeechEvent.c Star3d.c Stats.c StatScript.c StringSupport.c StubTransform.c Subtitle.c \
               Tactical.c Tactics.c Task.c TaskBar.c Teams.c Timer.c TitanNet.c Tracking.c TradeMgr.c Trails.c Transformer.c Tutor.c Tweak.c Twiddle.c Types.c UIControls.c Undo.c \
               Universe.c UnivUpdate.c Vector.c Volume.c

SDL_MODULES = avi.c dct.c font.c fqcodec.c fqeffect.c fquant.c main.c mainrgn.c mixfft.c mouse.c NetworkInterface.c prim2d.c prim3d.c Queue.c render.c rglu.c rinit.c screenshot.c \
              smixer.c soundlow.c sstream.c texreg.c TimeoutTimer.c Titan.c TitanInterfaceC.stub.c utility.c

MISSION_OBJECTS = Tutorial1 Mission01 Mission02 Mission03 Mission04 Mission05 Mission05_OEM Mission06 Mission07 Mission08 Mission09 Mission10 Mission11 \
                  Mission12 Mission13 Mission14 Mission15 Mission16

SRCDIR = ../src

MODULES = $(SDL_MODULES:%=$(SRCDIR)/SDL/%) \
          $(GAME_MODULES:%=$(SRCDIR)/Game/%) \
          $(wildcard $(SRCDIR)/Ships/*.c) \
          $(JPG_MODULES:%=$(SRCDIR)/ThirdParty/JPG/%) \
          $(SRCDIR)/ThirdParty/CRC/CRC32.c \
          $(wildcard $(SRCDIR)/ThirdParty/LZSS/*.c)

# -DHW_GAME_HOMEWORLD
# -DHW_GAME_DEMO
# -DHW_BUILD_FOR_DISTRIBUTION
# -DHW_BUILD_FOR_DEBUGGING

COMMONFLAGS = -MMD \
              -MP \
              -DHAVE_BOOLEAN \
              -DHW_GAME_HOMEWORLD \
              -DHW_BUILD_FOR_DISTRIBUTION \
              -DGENERIC_ETGCALLFUNCTION \
              -D_WIN32 \
              -D_LINUX_FIX_ME \
              -D_X86_64 \
              -D_X86_64_FIX_ME \
              -DWIN32 \
              -DWINDOWS \
              -DSTDC_HEADERS \
              -DGIT_VERSION="\"$(GIT_VERSION)\"" \
              -I $(BUILDDIR)/Missions/Generated/ \
              -I ../src/Game/ \
              -I ../src/Ships/ \
              -I ../src/Game/ \
              -I ../src/SDL/ \
              -I ../src/ThirdParty/CRC/ \
              -I ../src/ThirdParty/LZSS/ \
              -I ../src/ThirdParty/Titan/ \
              -I ../src/ThirdParty/JPG/ \
              -I .

EMXX = gcc
EMXXFLAGS = $(COMMONFLAGS) -O0 -ggdb3 `sdl2-config --cflags` -mconsole
EMXXLINK = -O0 -ggdb3 -lSDL2main -lSDL2 `sdl2-config --libs` -lopengl32 -lglew32 -mconsole


KAS2CFLAGS = -O0 -ggdb3 `sdl2-config --cflags` -lSDL2main -lSDL2 `sdl2-config --libs` -mconsole


all: kas2c missions .WAIT $(BINDIR)

# to generate windows pdb debug symbols from the mingw dwarf debug symbols
# download cv2pdb from https://github.com/rainers/cv2pdb and place the binaries
# in a subfolder called cv2pdb
debug: kas2c missions .WAIT $(BINDIR) $(BINDIR)/hw.pdb

LEX = lex
YACC = yacc

kas2c: ../tools/kas2c/kas2c.exe

../tools/kas2c/kas2c.exe: ../tools/kas2c/kas2c.c ../tools/kas2c/kas2c.h ../tools/kas2c/lexer.c ../tools/kas2c/parser.c
	$(EMXX) $(KAS2CFLAGS) ../tools/kas2c/*.c ../tools/kas2c/*.h -o $@

../tools/kas2c/lexer.c: ../tools/kas2c/lexer.l
	$(LEX) -i -o$@ ../tools/kas2c/lexer.l

../tools/kas2c/parser.c: ../tools/kas2c/parser.y
	$(YACC) -d -o $@ ../tools/kas2c/parser.y


missions: kas2c \
          $(MISSION_OBJECTS:%=$(BUILDDIR)/Missions/Generated/%.c)

.PRECIOUS: $(BUILDDIR)/Missions/Generated/%.c $(BUILDDIR)/Missions/Generated/%.h $(BUILDDIR)/Missions/Generated/%.func.c
$(BUILDDIR)/Missions/Generated/%.c $(BUILDDIR)/Missions/Generated/%.h $(BUILDDIR)/Missions/Generated/%.func.c: $(BUILDDIR)/Missions/Generated/%.kp ../tools/kas2c/kas2c.exe
	../tools/kas2c/kas2c.exe $< $(basename $@).c $(basename $@).h $(basename $@).func.c

.PRECIOUS: $(BUILDDIR)/Missions/Generated/Tutorial%.kp
$(BUILDDIR)/Missions/Generated/Tutorial%.kp: ../src/Missions/Tutorials/Tutorial%.kas
	@mkdir -p $(dir $@)
	cpp -x c $< > $@

.PRECIOUS: $(BUILDDIR)/Missions/Generated/Mission%.kp
$(BUILDDIR)/Missions/Generated/Mission%.kp: ../src/Missions/SinglePlayer/Mission%.kas
	@mkdir -p $(dir $@)
	cpp -x c $< > $@




$(BINDIR): $(BINDIR)/hw.exe

hw.res: hw.rc
	windres.exe hw.rc -O coff -o hw.res

$(BINDIR)/hw.exe: missions $(MODULES:%=$(BUILDDIR)/%.o) $(MISSION_OBJECTS:%=$(BUILDDIR)/Missions/Generated/%.o) $(MISSION_OBJECTS:%=$(BUILDDIR)/Missions/Generated/%.func.o) hw.res Makefile
	$(EMXX) $(filter %.o,$^) hw.res -o $@ $(EMXXLINK)

$(BUILDDIR)/Missions/Generated/%.o: $(BUILDDIR)/Missions/Generated/%.c
	@mkdir -p $(dir $@)
	@echo $<
	@$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: % Makefile
	@mkdir -p $(dir $@)
	@echo $<
	@$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(BINDIR)/hw.pdb: $(BINDIR)/hw.exe
	.\cv2pdb\cv2pdb.exe bin\hw.exe

clean:
	rm -rf build/* $(BUILDDIR)/* $(BINDIR)/hw.exe
