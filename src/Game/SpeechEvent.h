#ifndef ___SPEECHEVENT_H
#define ___SPEECHEVENT_H

// 03 = 00 00 00 11
// 3F = 00 11 11 11
// C0 = 11 00 00 00
// FC = 11 11 11 00
// FF = 11 11 11 11

#define SPEECH_EVENT_MASK                0x000003FF  // 1024 (incl. 0) possible speech events

// actor flags
#define SPEECH_ACTOR_PILOT               0x00000400
#define SPEECH_ACTOR_FLEET_COMMAND       0x00000800  // Karen; Mothership
#define SPEECH_ACTOR_FLEET_INTELLIGENCE  0x00001000
#define SPEECH_ACTOR_TRADERS             0x00002000  // Bentusi
#define SPEECH_ACTOR_P2_KADESHI          0x00004000  // "Garden" nebula inhabitants
#define SPEECH_ACTOR_ALL_ENEMY_SHIPS     0x00008000
#define SPEECH_ACTOR_AMBASSADOR          0x00010000  // Yours, sent to meet the Kadeshi
#define SPEECH_ACTOR_NARRATOR            0x00020000
#define SPEECH_ACTOR_DEFECTOR            0x00040000  // Captain Soban
#define SPEECH_ACTOR_GENERAL             0x00080000
#define SPEECH_ACTOR_EMPEROR             0x00100000
#define SPEECH_ACTOR_KHAR_SELIM          0x00200000

#define SPEECH_ACTOR_MASK                0x003FFC00

// speech type flags
#define SPEECH_TYPE_WHAT_IS_0x00400000   0x00400000  // not originally defined but some speech is flagged with it
#define SPEECH_TYPE_GROUP                0x00800000
#define SPEECH_TYPE_CHATTER              0x01000000
#define SPEECH_TYPE_STATUS               0x02000000
#define SPEECH_TYPE_COMMAND              0x04000000
#define SPEECH_TYPE_TUTORIAL             0x08000000
#define SPEECH_TYPE_SINGLE_PLAYER        0x10000000
#define SPEECH_TYPE_NIS                  0x20000000
#define SPEECH_TYPE_ANIMATIC             0x40000000
#define SPEECH_TYPE_ALWAYS_PLAY          0x80000000

#define SPEECH_TYPE_MASK                 0xFFC00000


/******************************************
****                                   ****
****             ALL SHIPS             ****
****                                   ****
******************************************/

#define FrickinForgotToStartAtZero				0

/* ALL SHIPS - Capital Ships */
#define STAT_Cap_Damaged_Specific				(1 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	1. STATUS_CapitalShip-Damaged(Report) (ALL SHIPS)
#define STAT_Cap_Surroundings_Enemy				(2 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	2. STATUS_CapitalShip-Surroundings(Report) (ALL SHIPS)
#define STAT_Cap_Surroundings_Resources			(3 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	2. STATUS_CapitalShip-Surroundings(Report) (ALL SHIPS)
#define STAT_Cap_Surroundings_Nothing			(4 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	2. STATUS_CapitalShip-Surroundings(Report) (ALL SHIPS)
#define STAT_Cap_LargeHit_Query					(5 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	7. STATUS_CapitalShip-LargeHit(Query) (ALL SHIPS)
#define STAT_Cap_LargeHit_More_Query			(6 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	7. STATUS_CapitalShip-LargeHit(Query) (ALL SHIPS)
#define STAT_Cap_LargeHit_Resp					(7 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	8. STATUS_CapitalShip-LargeHit(Response) (ALL SHIPS)
#define STAT_Cap_LargeHit_More_Resp				(8 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	8. STATUS_CapitalShip-LargeHit(Response) (ALL SHIPS)
#define STAT_Cap_Damaged						(9 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)		//	9. STATUS_CapitalShip-Damaged(Report) (ALL SHIPS)
#define STAT_Cap_Dies							(10 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	12. STATUS_CapitalShip_Dies (ALL SHIPS)
#define STAT_Cap_Dies_Report					(11 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	13. STATUS_CapitalShip-Dies('A'Report) (ALL SHIPS)
#define COMM_Cap_Retire							(12 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	15. COMMAND_CapitalShip-Retire (ALL SHIPS)

/* ALL SHIPS - Ship-Specific Events */
#define STAT_Strike_Damaged						(13 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	16. STATUS_StrikeCraft_Damaged (ALL SHIPS)
#define COMM_Strike_Retired						(14 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	17. COMMAND_StrikeCraft-Retired (ALL SHIPS)
#define COMM_SCVette_Salvage					(15 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	26. COMMAND_SalvageCorvette-Salvage
#define STAT_SCVette_TargetAcquired				(16 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	27. STATUS_SalvageCorvette-TargetAcquired (ALL SHIPS)
#define STAT_RepVette_StartedRepairs			(17 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	29. STATUS_RepairCorvette-StartedRepairs (ALL SHIPS)
#define STAT_RepVette_FinishedRepairs			(18 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	30. STATUS_RepairCorvette-FinishedRepairs (ALL SHIPS)
#define COMM_DDF_LaunchDrones					(19 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	33. COMMAND_DroneFrigate-LaunchDrones (ALL SHIPS)
#define COMM_DDF_RetractDrones					(20 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	34. COMMAND_DroneFrigate-RetractDrones (ALL SHIPS)
#define STAT_Carrier_Damaged					(21 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	36. STATUS_Carrier-Damaged(Report) (ALL SHIPS)
#define STAT_Cruiser_Damaged					(22 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	38. STATUS_HeavyCruiser-Damaged(Report) (ALL SHIPS)
#define STAT_MoShip_LargeAttack					(23 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	41. STATUS_MothershipUnderLargeAttack(Report) (ALL SHIPS)
#define STAT_MoShip_LargeAttack_Resp			(24 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	42. STATUS_MothershipUnderLargeAttack(Response) (ALL SHIPS)
#define COMM_Grav_On							(25 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	44. COMMAND_GravityWellGenerator-On (ALL SHIPS)
#define STAT_Grav_Collapse						(26 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	45. STATUS_GravityWellGenerator-Collapse (ALL SHIPS)
#define COMM_Cloak_CloakingOn					(27 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	47. COMMAND_CloakingShips-CloakingOn(Report) (ALL SHIPS)
#define STAT_Cloak_CloakingOn_Resp				(28 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	48. STATUS_CloakingShips-CloakingOn(Response) (ALL SHIPS)
#define COMM_Cloak_InsufficientPower			(29 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	49. COMMAND_CloakingShips-InsufficientPowerToCloak (ALL SHIPS)
#define STAT_Cloak_CloakPowerLow				(30 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	50. STATUS_CloakingShips-CloakingPowerLow(Report) (ALL SHIPS)
#define COMM_Cloak_Decloak						(31 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	51. COMMAND_CloakingShips-DeCloak (ALL SHIPS)

/* ALL SHIPS - Resourcing */
#define STAT_ResCol_Grp_Damaged					(32 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	52. STATUS_GroupCollectors-Damaged(Report) (ALL SHIPS)
#define STAT_ResCol_Destroyed					(33 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	53. STATUS_ResourceCollector-Destroyed(Report) (ALL SHIPS)
#define STAT_ResCol_ResourcesTransferred		(34 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	54. STATUS_ResourceCollector-ResourcesTransferred (ALL SHIPS)
#define COMM_ResCol_Harvest						(35 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	55. COMMAND_ResourceCollector-Harvest (ALL SHIPS)
#define STAT_ResCol_Full						(36 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	57. STATUS_ResourceCollector-Full (ALL SHIPS)
#define STAT_ResCol_Damaged						(37 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	58. STATUS_ResourceController-Damaged (ALL SHIPS)

/* ALL SHIPS - Dogfighting*/
#define STAT_Fighter_WingmanChased				(38 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	60. STATUS_Fighter-WingmanChased(Report) (ALL SHIPS)
#define STAT_Fighter_WingmanChased_Rsp			(39 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	61. STATUS_Fighter-WingmanChased(Response) (ALL SHIPS)
#define STAT_Fighter_WingmanHit					(40 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	62. STATUS_Fighter-WingmanHit(Report) (ALL SHIPS)
#define STAT_Fighter_WingmanHit_Rsp				(41 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	63. STATUS_Fighter-WingmanHit(Response) (ALL SHIPS)
#define STAT_Fighter_WingmanLethal				(42 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	65. STATUS_Fighter-WingmanLethallyDamaged(Response) (ALL SHIPS)
#define STAT_Fighter_WingmanDies				(43 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	66. STATUS_Fighter-WingmanDies(Report) (ALL SHIPS)
#define STAT_Fighter_LeaderChased				(44 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	68. STATUS_Fighter-LeaderBeingChased(Report) (ALL SHIPS)
#define CHAT_Fighter_Wingman					(45 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	67. CHATTER_Fighter-Wingman(Report) (ALL SHIPS)
#define COMM_LInt_Kamikaze						(46 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	69. COMMAND-LightInterceptor-Kamikaze (ALL SHIPS)

/* ALL SHIPS - Battle Events */
#define STAT_AssGrp_UnderAttack					(47 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	70. STATUS_AssignedGroup-UnderAttack(Report) (ALL SHIPS)
#define STAT_Grp_UnderAttack					(48 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	71. STATUS_GenericGroup-UnderAttack(Report) (ALL SHIPS)
#define STAT_Grp_UnderAttack_Rsp				(49 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	72. STATUS_GenericGroup-UnderAttack(Response) (ALL SHIPS)
#define STAT_Grp_StartBattleDisadvant			(50 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	73. STATUS_GenericGroup-StartBattleDisadvantaged(Report) (ALL SHIPS)
#define STAT_Grp_StartBattleDisadvant_Rsp		(51 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	74. STATUS_GenericGroup-StartBattleDisadvantaged(Response) (ALL SHIPS)
#define STAT_Grp_StartBattleFair				(52 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	75. STATUS_GenericGroup-StartBattleFair(Report) (ALL SHIPS)
#define STAT_Grp_StartBattleFair_Rsp			(53 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	76. STATUS_GenericGroup-StartBattleFair(Response) (ALL SHIPS)
#define STAT_Grp_StartBattleAdvantaged			(54 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	77. STATUS_GenericGroup-StartBattleAdvantaged(Report) (ALL SHIPS)
#define STAT_Grp_StartBattleAdvantaged_Rsp		(55 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	78. STATUS_GenericGroup-StartBattleAdvantaged(Response) (ALL SHIPS)
#define COMM_Group_Retreat						(56 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	79. COMMAND_GenericGroup-Retreat (ALL SHIPS)
#define COMM_Group_Retreat_Rsp					(57 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	80. COMMAND_GenericGroup-Retreat(Response) (ALL SHIPS)
#define STAT_Grp_WinningBattle					(58 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	81. STATUS_GenericGroup-WinningBattle(Report) (ALL SHIPS)
#define STAT_Grp_WinningBattle_Rsp				(59 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	82. STATUS_GenericGroup-WinningBattle(Response) (ALL SHIPS)
#define STAT_Grp_KickingButt					(60 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	83. STATUS_GenericGroup-KickingButt(Report) (ALL SHIPS)
#define STAT_Grp_KickingButt_Rsp				(61 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	84. STATUS_GenericGroup-KickingButt(Response) (ALL SHIPS)
#define STAT_Grp_BattleWon						(62 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	85. STATUS_GenericGroup-BattleWon(Report) (ALL SHIPS)
#define STAT_Grp_LosingBattle					(63 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	86. STATUS_GenericGroup-LosingBattle(Report) (ALL SHIPS)
#define STAT_Grp_LosingBattle_More				(64 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	86. STATUS_GenericGroup-LosingBattle(Report) (ALL SHIPS)
#define STAT_Grp_LosingBattle_Rsp				(65 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	87. STATUS_GenericGroup-LosingBattle(Response) (ALL SHIPS)
#define STAT_Grp_LosingBadly					(66 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	88. STATUS_GenericGroup-LosingBadly(Report) (ALL SHIPS)
#define STAT_Grp_LosingBadly_More				(67 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	88. STATUS_GenericGroup-LosingBadly(Report) (ALL SHIPS)
#define STAT_Grp_LosingBadly_Rsp				(68 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	89. STATUS_GenericGroup-LosingBadly(Response) (ALL SHIPS)
#define CHAT_Grp_PositiveBattle					(69 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	90. CHATTER_GenericGroup-PositiveBattle(Report) (ALL SHIPS)
#define CHAT_Grp_PositiveBattle_More			(70 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	90. CHATTER_GenericGroup-PositiveBattle(Report) (ALL SHIPS)
#define CHAT_Grp_NegativeBattle					(71 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	91. CHATTER_GenericGroup-NegativeBattle(Report) (ALL SHIPS)
#define CHAT_Grp_NegativeBattle_More			(72 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	91. CHATTER_GenericGroup-NegativeBattle(Report) (ALL SHIPS)
#define CHAT_Grp_NegativeBattle_More2			(73 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	91. CHATTER_GenericGroup-NegativeBattle(Report) (ALL SHIPS)
#define CHAT_Grp_NeutralBattle					(74 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	92. CHATTER_GenericGroup-NeutralBattle(Report) (ALL SHIPS)
#define CHAT_Grp_NeutralBattle_More				(75 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	92. CHATTER_GenericGroup-NeutralBattle(Report) (ALL SHIPS)
#define CHAT_Grp_FriendlyFire					(76 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	93. CHATTER_GenericGroup-FriendlyFire (ALL SHIPS)

/* ALL SHIPS - Selection, Movement and Other Commands */
#define COMM_AssGrp_Select						(77 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND + SPEECH_TYPE_GROUP)	//	95. COMMAND_AnyShip-SelectHotkeyGroup (ALL SHIPS)
#define COMM_Selection							(78 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	96. COMMAND_AnyShip-Selection (ALL SHIPS)
#define COMM_Move								(79 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	97. COMMAND_AnyShip-SpecifyDestination (ALL SHIPS)
#define COMM_MoveCancelled						(80 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	98. COMMAND_AnyShip-MoveCancelled (ALL SHIPS)
#define COMM_Attack								(81 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	102. COMMAND_AnyShip-Attack (ALL SHIPS)
#define COMM_AttackCancelled					(82 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	103. COMMAND_AnyShip-AttackCancelled (ALL SHIPS)
#define COMM_AttackPlayerUnits					(83 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	105. COMMAND_AnyShip-ForceAttackPlayerUnits (ALL SHIPS)
#define COMM_AttackPlayerUnits_Cond				(84 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	106. COMMAND_AnyShip-ForceAttackPlayerUnits(Conditional) (ALL SHIPS)
#define COMM_AttackResources					(85 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	107. COMMAND_AnyShip-ForceAttackResources (ALL SHIPS)
#define COMM_AttackResourcesGeneric				(86 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	108. COMMAND_AnyShip-ForceAttackResourcesGeneric (ALL SHIPS)
#define COMM_SetFormation						(87 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	111. COMMAND_AnyShip-SetFormation (ALL SHIPS)
#define COMM_Guard								(88 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	113. COMMAND_AnyShip-Guard (ALL SHIPS)
#define STAT_Strike_LowOnFuel					(89 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	115. STATUS_StrikeCraft-LowOnFuel (ALL SHIPS)
#define STAT_Strike_OutOfFuel					(90 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	116. STATUS_StrikeCraft-OutOfFuel (ALL SHIPS)
#define COMM_CannotComply						(91 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	119. COMMAND_AnyShip-CannotComply (ALL SHIPS)

/* ALL SHIPS - Hyperspace */
#define COMM_Hyper_Autodock						(92 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	120. COMMAND_AnyShip-Hyperspace&Autodock (ALL SHIPS)
#define STAT_Hyper_Abandoned					(93 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	122. STATUS_AnyShip-AbandonedByHyperspace (ALL SHIPS)

/* ALL SHIPS - Construction and Docking */
#define CHAT_Const_DuringBattle					(94 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	123. CHATTER_CarrierOrMShip-ConstructionDuringBattle (ALL SHIPS)
#define COMM_Const_BuildCapShip					(95 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	124. COMMAND_CarrierOrMShip-BuildCapitalShip (ALL SHIPS)
#define CHAT_Const								(96 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	125. CHATTER_CarrierOrMShip-Construction (ALL SHIPS)
#define CHAT_Const_More							(97 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	125. CHATTER_CarrierOrMShip-Construction (ALL SHIPS)
#define STAT_Cap_Launched						(98 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	126. STATUS_CarrierOrMShip-CapitalShipLaunched (ALL SHIPS)
#define STAT_Cap_Welcomed						(99 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	127. STATUS_AnyShip-CapitalShipWelcomed (ALL SHIPS)
#define COMM_Strike_Dock						(100 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	128. COMMAND_StrikeCraft-Dock (ALL SHIPS)
#define CHAT_Docking							(101 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	130. CHATTER_AnyShip-Docking (ALL SHIPS)
#define CHAT_Docking_More						(102 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_CHATTER)	//	130. CHATTER_AnyShip-Docking (ALL SHIPS)
#define STAT_Cap_ShipDocked						(103 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	131. STATUS_CapitalShips-ShipDocked (ALL SHIPS)
#define STAT_Research_Docking					(104 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	133. STATUS_ResearchShip-Docking (ALL SHIPS)
#define STAT_Research_Docked					(105 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	134. STATUS_ResearchShip-Docked (ALL SHIPS)

/* ALL SHIPS - New Events */
#define COMM_ResCol_NoMoreRUs					(106 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	136. COMMAND_ResourceCollector-NoMoreRUs
#define STAT_Strike_AlsoOutOfFuel				(107 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	138. STATUS_StrikeCraft-AlsoOutofFuel
#define STAT_Strike_GenericUsToo				(108 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	139. STATUS_StrikeCraft-GenericUsToo
#define	STAT_Strike_StuckInGrav					(109 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	140. STATUS_StrikeCraft-StuckbyGravWell
#define STAT_Strike_AttackComplete				(110 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	141. STATUS_ StrikeCraft-AttackComplete
#define	COMM_TacticsEvasive						(111 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	142. COMMAND_AnyShip-SetTacticsEvasive
#define COMM_TacticsNeutral						(112 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	143. COMMAND_AnyShip-SetTacticsNeutral
#define COMM_TacticsAggressive					(113 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	144. COMMAND_AnyShip-SetTacticsAggressive
#define COMM_Support							(114 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	145. COMMAND_SupportShip-Support
#define COMM_Grav_Off							(115 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	146. COMMAND_GravityWellGenerator-Off
#define COMM_HVette_BurstAttack					(116 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	147. COMMAND_HeavyCorvette-BurstAttack
#define COMM_MLVette_ForceDrop					(117 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	148. COMMAND_MinelayerCorvette-ForceDropMines
#define COMM_MissleDest_VolleyAttack			(118 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	149. COMMAND_MissileDestroyer-VolleyAttack
#define COMM_Strike_OutOfFuelMove				(119 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	150. COMMAND_StrikeCraft-OutOfFuelMove
#define COMM_Kamikaze_NoTargets					(120 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_COMMAND)	//	151. COMMAND_AnyShip-KamikazeNoTargets
#define STAT_Grp_RetreatSuccessful				(121 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	152. STATUS_GenericGroup-RetreatSuccessful
#define STAT_AssGrp_RetreatSuccessful			(122 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	153. STATUS_AssignedGroup-RetreatSuccessful
#define STAT_Grp_EnemyRetreated					(123 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	154. STATUS_GenericGroup-EnemyRetreated
#define STAT_AssGrp_EnemyRetreated				(124 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	155. STATUS_AssignedGroup-EnemyRetreated
#define STAT_Grp_AttackFromAlly					(125 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	156. STATUS_Group-UnderAttackFromAlly
#define STAT_AssGrp_FriendlyFire				(126 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	159. STATUS_AssignedGroup-FriendlyFire
#define STAT_SCVette_DroppingOff				(127 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	160. STATUS_SalvageCorvette-DroppingOff
#define STAT_Grp_AttackRetaliate				(128 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	161. STATUS_GenericGroup-UnderAttackRetaliate(Report)
#define STAT_Grp_AttackRetaliate_Repeat			(129 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	162. STATUS_GenericGroup-UnderAttackRetaliate(Repeat)
#define STAT_AssGrp_AttackRetaliate				(130 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	163. STATUS_AssignedGroup-UnderAttackRetaliate(Report)
#define STAT_AssGrp_AttackRetaliate_Repeat		(131 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	164. STATUS_AssignedGroup-UnderAttackRetaliate(Report)
#define STAT_Grp_InMineField					(132 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	166. STATUS_GenericGroup-InMineField
#define	STAT_AssGrp_InMineField					(133 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	167. STATUS_AssignedGroup-InMineField
#define STAT_Research_StationOnline				(134 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	170. STATUS_ResearchShip-StationOnline

/* ALL SHIPS - Appendix */
#define STAT_Grp_EnemyFightersDecloaking		(135 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	STATUS_GenericGroup_EnemyCloakedFightersDecloaking
#define STAT_Grp_EnemyGeneratorDecloaking		(136 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	STATUS_GenericGroup_EnemyCloakGeneratorDecloaking



/******************************************
****                                   ****
****           FLEET COMMAND           ****
****                                   ****
******************************************/

/* FLEET COMMAND - Dialogues and Warnings */
#define STAT_F_AssGrp_UnderAttack				(137 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	1. STATUS_AssignedGroup-UnderAttack (FLEET)
#define COMM_F_Group_Assigning					(138 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	2. COMMAND_HotkeyGroups-Assigning (FLEET)
#define STAT_F_AssGrp_Victory					(139 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	3. STATUS_HotkeyGroup-Victory (FLEET)
#define STAT_F_AssGrp_Defeat					(140 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP)	//	5. STATUS_HotkeyGroup-Defeat (FLEET)
#define COMM_F_Probe_Deployed					(141 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	6. COMMAND_Probe-Deployed (FLEET)
#define STAT_F_Probe_Arrived					(142 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	7. STATUS_Probe-Arrived (FLEET)
#define STAT_F_Probe_Expiring					(143 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	8. STATUS_Probe-Expiring (FLEET)
#define STAT_F_EnemyProbe_Detected				(144 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	9. STATUS_EnemyProbe-Detected (FLEET)
#define STAT_F_ProxSensor_Detection				(145 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	10. STATUS_ProximitySensor-Detection (FLEET)
#define STAT_F_ResCol_Damaged					(146 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	11. STATUS_ResourceCollector-Damaged(Singular) (FLEET)
#define STAT_F_ResCol_Dies						(147 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	12. STATUS_ResourceCollector-Dies (FLEET)
#define STAT_F_ResLevels_Nil					(148 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	14. STATUS_ResourceLevels-Nil (FLEET)
#define STAT_F_SuperCap_Damaged					(149 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	15. STATUS_SuperCapitalShip-Damaged (FLEET)
#define STAT_F_Cap_Dies							(150 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	16. STATUS_CapitalShip-Dies (FLEET)
#define STAT_F_MoShip_UnderAttack				(151 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	17. STATUS_Mothership-UnderAttack (FLEET)
#define COMM_F_MoShip_Move						(152 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	18. COMMAND_Mothership-Move (FLEET)
#define COMM_F_MoShip_Arrived					(153 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND + SPEECH_TYPE_ALWAYS_PLAY)	//	19. COMMAND_Mothership-ArrivesAtDestination (FLEET)
#define COMM_F_Hyper_Engage						(154 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	20. COMMAND_Hyperspace_Engage (FLEET)
#define STAT_F_Hyper_Autodock					(155 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	21. STATUS_Hyperspace-Autodock (FLEET)
#define STAT_F_Hyper_TakingOff					(156 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	22. STATUS_Hyperspace-TakingOff (FLEET)
#define COMM_F_Hyper_Abort						(157 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	23. COMMAND_Hyperspace-Abort (FLEET)

/* FLEET COMMAND - Construction */
#define COMM_F_Const_Start						(158 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	27. COMMAND_StartConstruction (FLEET)
#define COMM_F_Const_Pause						(159 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	28. COMMAND_PauseConstruction (FLEET)
#define STAT_F_ConstInProgs_ResInsufficient		(160 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	29. STATUS_ConstructionInProgress-ResourceInsufficient (FLEET)
#define STAT_F_Const_UnitLimit					(161 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	31. STATUS_Construction-ShipUnitLimitHit (FLEET)
#define STAT_F_Const_TotalLimit					(162 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	32. STATUS_Construction-TotalUnitLimitHit (FLEET)
#define COMM_F_Const_CancelAll					(163 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	33. COMMAND_CancelAllJobs (FLEET)
#define COMM_F_Const_CancelBatch				(164 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	34. COMMAND_CancelBatch (FLEET)
#define COMM_F_Const_Resume						(165 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	36. COMMAND_ResumeConstruction (FLEET)
#define STAT_F_Const_Complete					(166 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	37. STATUS_ConstructionComplete (FLEET)
#define STAT_F_Const_CapShipComplete			(167 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	38. STATUS_Construction-CapitalShipComplete (FLEET)

/* FLEET COMMAND - Launching */
#define COMM_F_Launch							(168 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	39. COMMAND_Launch (FLEET)
#define COMM_F_AutolaunchOn						(169 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	40. COMMAND_Autolaunching-On (FLEET)

/* FLEET COMMAND - Research */
#define COMM_F_Research_R1_Start				(170 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	41. COMMAND_Research-Start (FLEET)
#define COMM_F_Research_R2_Start				(171 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	41. COMMAND_Research-Start (FLEET)
#define COMM_F_Research_Stop					(172 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	42. COMMAND_Research-Stop (FLEET)
#define STAT_F_Research_R1_NearCompletion		(173 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	43. STATUS_Research-NearingCompletion (FLEET)
#define STAT_F_Research_R2_NearCompletion		(174 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	43. STATUS_Research-NearingCompletion (FLEET)
#define STAT_F_Research_R1_Completed			(175 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	44. STATUS_Research-Completed (FLEET)
#define STAT_F_Research_R2_Completed			(176 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	44. STATUS_Research-Completed (FLEET)
#define STAT_F_Research_CompletedShip			(177 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	45. STATUS_Research-CompletedShip (FLEET)

/* FLEET COMMAND - New Events */
#define COMM_F_ScuttleReaffirm					(178 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	48. COMMAND_ScuttleReaffirm
#define COMM_F_AssGrp_AddingShips				(179 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND + SPEECH_TYPE_GROUP)	//	49. COMMAND_AssignedGroup-AddingShips
#define STAT_F_ResourcesAllHarvested			(180 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	50. STATUS_ResourcesAllHarvested
#define STAT_F_ReassignedToCarrier				(181 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	51. STATUS_FleetCommandReassignedtoCarrier
#define STAT_F_GravWellDetected					(182 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	52. STATUS_GravWellDetected
#define STAT_F_CloakedShipsDetected				(183 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	53. STATUS_CloakedShipsDetected
#define STAT_F_EnemyShipCaptured				(184 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	54. STATUS_EnemyShipCaptured
#define STAT_F_ShipStolen						(185 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	55. STATUS_ShipStolen
#define STAT_F_ResourceInjectionRecieved		(186 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS + SPEECH_TYPE_ALWAYS_PLAY)	//	56. STATUS_ResourceInjectionReceived
#define STAT_F_UnderAttackFromAlly				(187 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	58. STATUS_UnderAttackFromAlly
#define STAT_F_Cap_Retired						(188 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	59. STATUS_CapitalShipRetired
#define STAT_F_Cap_Retired_PercentRecovered		(189 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	59. STATUS_CapitalShipRetired
#define COMM_F_AllianceFormed					(190 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND + SPEECH_TYPE_ALWAYS_PLAY)	//	61. COMMAND_AllianceFormed
#define COMM_F_AllianceBroken					(191 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND + SPEECH_TYPE_ALWAYS_PLAY)	//	62. COMMAND_AllianceBroken
#define COMM_F_ResourcesTransferred				(192 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND + SPEECH_TYPE_ALWAYS_PLAY)	//	63. COMMAND_ResourcesTransferred
#define COMM_F_ResourcesReceived				(193 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND + SPEECH_TYPE_ALWAYS_PLAY)	//	64. COMMAND_ResourcesReceived

/* FLEET COMMAND - Single-Player Events:*/
#define STAT_F_HyperspaceSuccessful				(194 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	70. STATUS_HyperspaceSuccessful NEW
#define STAT_F_HyperspaceInterrupted			(195 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	70.5 STATUS_HyperspaceInterrupted NEW
#define STAT_F_HyperspaceDrivesAvailable		(196 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	71. STATUS_HyperspaceDrivesAvailable NEW

/* FLEET COMMAND - Appendix */
#define COMM_F_BuildCenterSelected				(197 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	COMMAND_BuildCentreSelected
#define COMM_F_MoShip_Selected					(198 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	COMMAND_MothershipSelected
#define COMM_F_MoShip_CantMove					(199 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	COMMAND_MothershipCantMove


/******************************************
****                                   ****
****             TUTORIAL              ****
****                                   ****
******************************************/

#define TUT_Intro								(200 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Welcome to the Homeworld Training Mission.  Here we will teach you all the basic controls needed to play Homeworld. Lets begin!  Leftclick on the Next button to proceed.

/* Lesson 1: Using the Camera */
#define TUT_SaveCamera							(201 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Using the camera.  Click the Next button to begin.
#define TUT_CameraMoveDescription				(202 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	To rotate the camera, hold the right mouse button and drag the mouse.  Click Next to continue.
#define TUT_CameraMove							(203 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	To advance to the next lesson, rotate the camera 180 degrees around your Mothership.
#define TUT_CameraMoveUp						(204 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Next rotate the camera upward to view the Mothership from above.
#define TUT_CameraMoveDown						(205 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now view the Mothership from below.
#define TUT_CameraBackToMiddle					(206 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Great!  Move the camera back to the horizontal.  Then I'll show you how to zoom in and out.
#define TUT_CameraZoomDescription				(207 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	To zoom in or out, hold both mouse buttons and drag the mouse up or down.  If your mouse has a wheel, you can use the wheel to zoom as well.  Click Next to continue.
#define TUT_CameraZoomOut						(208 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Zoom the camera out as far as you can.
#define TUT_CameraZoomIn						(209 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	From this position, we get a good overview of the action.  Now zoom the camera in as far as you can.
#define TUT_CameraPractice						(210 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Excellent.  From this position we get a good close-up view of the action.  Practice rotating and zooming the camera.  When you're ready to move on, click the Next button.

/* Lesson 2: TitleBuild */
#define TUT_TaskbarSave							(211 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Building Ships.  Click Next to begin.
#define TUT_TaskbarIntro						(212 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Most of Homeworld's user controls are accessible from a popup taskbar.  Move your cursor to the bottom of the screen to see the taskbar.
#define TUT_TaskbarDescription					(213 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	There it is.  Normally the taskbar would disappear when you move your cursor away from it, but let's keep it up to take a closer look.  Click Next to continue.
#define TUT_TaskbarBuildManagerButton			(214 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This button brings up the Build Manager.  Leftclick on the Build button now.

/* Lesson 3: Building Ships */
#define TUT_BuildManagerIntro					(215 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Welcome to the Build Manager.  Here you can build new ships for the fleet.  Click Next to continue.
#define TUT_BuildManagerRUCount					(216 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This is your RU count.  It displays the amount of Resource Units available for building.  Click Next to continue.
#define TUT_BuildManagerShipList				(217 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This is the ship list.  It displays the ships available for building, their cost, and the number of ships in each build queue.  Click Next to continue.
#define TUT_BuildManagerAddResCollector			(218 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Let's build some ships.  Leftclick on the Resource Collector to select it.
#define TUT_3_4_BuildManagerIncrease			(219 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Leftclick again on the Resource Collector to add one to the build queue.
#define TUT_BuildManagerBuildRC					(220 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Leftclick on the Build button to begin construction.
#define TUT_BuildManagerIDProgressBar			(221 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Notice the progress bar.  This indicates how much of the ship has been built.  Click Next to continue.
#define TUT_BuildManagerBuildResearchShip		(222 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now add a Research Ship to the build queue.  Leftclick twice on the Research Ship...
#define TUT_BuildManagerHitBuildRS				(223 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	...then leftclick on the Build button.
#define TUT_BuildManagerBuildScouts				(224 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now add 10 scouts to the build queue.
#define TUT_BuildManagerHitBuildScouts			(225 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Good.  Now leftclick on the Build button to begin construction.
#define TUT_BuildManagerProgressBars			(226 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Take a close look at the progress bar.  The darker top bar shows the progress of the ship currently being built. Click Next to continue.
#define TUT_3_12_BuildManagerBottomProgressBar	(227 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	And the lighter bottom bar shows the progress of the entire batch. Click Next to continue.
#define TUT_BuildManagerClose					(228 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	To exit the Build Manager, leftclick on the Close button or press (ESCAPE).  Construction will continue.

/* Lesson 4: Selecting Ships */
#define TUT_SaveSelectingShips					(229 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Selecting and Focusing.   Click Next to begin.
#define TUT_4_1_SelectIntroZoomOut				(230 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Zoom out to bring the entire Mothership into view.
#define TUT_SelectIntro							(231 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Select the Mothership by leftclicking on it.
#define TUT_4_5_DeselectMothership				(232 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	De-select it by leftclicking on any empty space.
#define TUT_BandboxMothership					(233 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Hold down the left mouse button and drag a selection box around the Mothership.
#define TUT_SelectInfoOverlay					(234 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	At the top right of the screen is the Info Overlay.  It displays all the selected ships.  To proceed, zoom the camera all the way out.
#define TUT_SelectBandboxAll					(235 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now drag a single selection box around as many ships as you can.
#define TUT_SelectOverlayClick					(236 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Notice how the Info Overlay lists all the selected ships.  Click on the Resource Collector in the list to select it.
#define TUT_SelectFocusResCollect				(237 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now the Resource Collector is selected.  Click next to continue.
#define Shanes_A_Dummy							238
#define Shanes_A_Big_Dummy						239
#define TUT_4_12_FocusResCollect				(240 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Homeworld's camera always rotates around a ship or group of ships.  This is called the camera's focus.  To focus the camera on the Resource Collector, click your middle mouse button or press the (F) key.
#define TUT_SelectMoveCameraFocus				(241 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Good.  Rotate the camera.  Zoom it in and out.  Notice the camera is centered around the Resource Collector.  Click next to continue.
#define TUT_4_14_CameraFocusAgain				(242 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Click the middle mouse button again, or press the (F) key again to zoom in as close as possible.
#define TUT_SelectFocusIn						(243 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Zoom the camera out again to continue.
#define TUT_4_16_SelectScouts					(244 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	You have a line of Scouts in front of the Mothership.  Hold down the left mouse button and drag a selection box around all of them.
#define TUT_4_17_SelectFocusScouts				(245 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Click the middle mouse button or press the (F) key.
#define TUT_4_18_ScoutsCenteredFocusIn			(246 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now the camera is focused on all your Scouts.  Click the middle mouse button or press the (F) key again.
#define TUT_4_19_ScoutsCloseup					(247 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now we're zoomed in on the Scouts.  Move the camera around to take a look at them.
#define TUT_SelectFocusPractice					(248 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Take some time to practice selecting and focusing.  When the action heats up, it's important to be comfortable with these controls.  When you're ready to move on, click the Next button.
#define TUT_SelectResourceCollectorFirst		(249 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Select the Resource Collector, focus on it and zoom in until you're reasonably close.
#define TUT_SelectResCollHealthBar				(250 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This is the ship's health bar.  It indicates the ship is selected.  If the ship gets damaged, the green bar will shrink and change color.  Click Next to continue.
#define TUT_SelectResCollContextMenu			(251 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	When selected, a ship's functions are accessible through its rightclick menu.  Right click on the Resource Collector to bring up this menu.
#define TUT_SelectResCollDescribeContextMenu	(252 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Begin harvesting by leftclicking on Harvest in the menu.
#define TUT_SelectResCollCollecting				(253 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The Resource Collector will now find the nearest resources to harvest.  Note that you can still rotate and zoom the camera while it moves.  Click next to continue.
#define TUT_SelectFocusResearchShip				(254 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	To move on to the next lesson, select and focus on the Research Ship.  To help you find it, I have drawn a pointer to it.

/* Lesson 5: TitleResearch */
#define TUT_SaveResearch						(255 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Researching Technology.  Click Next to begin.
#define TUT_ResearchIntro						(256 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This is a Research Ship.  The more you have of these, the faster you can research.  To access the Research Manager, bring up the taskbar by moving your cursor to the bottom of the screen.
#define TUT_TaskbarResearchManagerButton		(257 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This button brings up the Research Manager.  Leftclick on it now.
#define TUT_ResearchManagerScreen				(258 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Welcome to the Research Manager.  Here you can research technology which will make new ship types available for construction.  Click Next to continue.
#define TUT_ResearchManagerTopicWindow			(259 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This window is the technology list window.  Here you select the technologies to research.  Click Next to continue.
#define TUT_5_4_ResearchManagerGreenDot			(260 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The green dot indicates that Capital Ship Drive technology has already been researched.
#define TUT_5_5_ResearchManagerSelectTech		(261 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Leftclick on the Capital Ship Chassis technology to select it.
#define TUT_ResearchManagerTopicDescription		(262 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Leftclick on the Research button.
#define TUT_ResearchManagerResearching			(263 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Notice that the technology has a progress bar.  This indicates how much research has been done.  Click Next to continue.
#define TUT_5_8_ResearchManagerClose			(264 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Leftclick on the Close button to exit the Research Manager.

/* Lesson 6: Moving Ships */
#define TUT_SaveMovingShips						(265 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Moving Ships.  Click Next to begin.
#define TUT_MoveSelectScouts					(266 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now it's time to move your 10 Scouts.  Zoom out and select them all.
#define TUT_MoveFocusScouts						(267 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Focus on the scouts.
#define TUT_MoveCameraOverview					(268 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now rotate the camera so we're looking straight down on the Scouts.
#define TUT_MoveCameraZoomOut					(269 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Lastly, zoom out as far as you can.
#define TUT_MoveContextMenu						(270 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This gives us a nice view for moving the ships.  Rightclick on one of the Scouts to bring up its menu.
#define TUT_MoveSelectMove						(271 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Select the Move option from the menu.
#define TUT_MovePizzaDishIntro					(272 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This is the Movement Disc.  Leftclick anywhere on the screen to move the Scouts there.
#define TUT_MoveScoutsMove						(273 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Moving ships is very important, so let's try it again.  Click Next to continue.
#define TUT_MoveHitM							(274 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	As a shortcut, bring up the Movement Disc by pressing the (M) key.
#define TUT_MoveCameraPizzaDish					(275 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This time, rotate and zoom the camera before issuing the move command.
#define TUT_6_10_MoveHitM2						(276 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now let's try some 3D movement.  Bring up the Movement Disc again.
#define TUT_MoveUp								(277 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Hold the (SHIFT) key and drag the mouse up or down to change the height of your destination point.  When you have found an acceptable height, release the (SHIFT) key.
#define TUT_6_14_MoveUpRelease					(278 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now the height of the destination is fixed.  Drag the mouse to move the destination cursor at this level.
#define TUT_6_15_MovePractice					(279 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Nice work!  Take some time to practice horizontal and vertical movement.  When you feel comfortable with these controls, click Next.

/* Lesson 7: TitleSM */
#define TUT_SMSave								(280 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The Sensors Manager.  Click Next to begin.
#define TUT_SMIntro								(281 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The Sensors Manager is Homeworld's large-scale map.  To access it, first bring up the taskbar at the bottom of your screen.
#define TUT_SMPressSensors						(282 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Leftclick on this button to enter the Sensors Manager.
#define TUT_SMSensorsManager					(283 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Welcome to the Sensors Manager.  >From here you can get an overview of your surroundings and move your ships long distances.  Notice that you can still rotate and zoom the camera.  Click the Next button to continue.
#define TUT_SMDescription						(284 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This is your Mothership.
#define TUT_7_4_SMDescriptionGreenDot			(285 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The green points represent your ships.
#define TUT_7_5_SMDescriptionRedDot				(286 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The red points represent enemy ships.
#define TUT_7_6_SMDescriptionLgBrownDot			(287 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The large brown points represent harvestable resources.
#define TUT_7_7_SMDescriptionSmBrownDot			(288 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The small brown points represent space dust that cannot be collected.
#define TUT_SMBandBox							(289 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Leftclick on any of your ships to exit the Sensors Manager and move the camera to that position.
#define TUT_SMExit								(290 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	In this way you can cover large distances quickly.  Use the taskbar or press (SPACE) to get back into the Sensors Manager.
#define TUT_SMSelected							(291 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Selected ships are displayed as flashing points in the Sensors Manager.  Click Next to continue.
#define TUT_7_12_SMMoveButton					(292 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The Sensors Manager allows you to move ships the same way you do normally.
#define TUT_SMFocus								(293 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Focusing also works normally in the Sensors Manager.  Press (F) or click the middle mouse button to focus on the selected ships.
#define TUT_SMPractice							(294 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	You've learned a lot.  Practice moving your Scouts and using the Sensors Manager.  When you are ready, focus on the Scouts and click Next.

/* Lesson 8: TitleFormations */
#define TUT_SaveFormation						(295 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Formations. Click Next to begin.
#define TUT_FormationIntro						(296 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Right click on the scouts to bring up their menu.
#define TUT_FormationDelta						(297 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Move your cursor to Formation and then leftclick on Delta.
#define TUT_FormationDeltaBroad					(298 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now the fighters are in the Delta formation.  Use the rightclick menu again to assign Broad formation.
#define TUT_FormationBroadX						(299 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Good.  Next try Formation X.
#define TUT_FormationXClaw						(300 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Formations can also be assigned using the (TAB) key.  Press (TAB) to set Claw formation.
#define TUT_FormationClawWall					(301 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Notice that (TAB) will cycle to the next formation.  Press (TAB) again to see how.
#define TUT_FormationWallSphere					(302 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	This is the Wall.  Press (TAB) for Sphere formation.
#define TUT_FormationPractice					(303 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Take some time to familiarize yourself with the formations.  When you're ready to continue, put the Scouts into Claw formation and leftclick on the Next button.
#define TUT_8_9_LookForEnemy					(304 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	To begin the combat lesson, use the Sensors Manager to move your Scouts near the enemy ships.
#define TUT_8_10_RedDots						(305 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	These are the enemies.  Move your Scouts here.
#define TUT_8_11_LeaveSensors					(306 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Once you have issued the move, you can leave the Sensors Manager.

/* Lesson 9: TitleAttacking */
#define TUT_AttackingSave						(307 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Combat.  Click Next to begin.
#define TUT_AttackingMouseOver					(308 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Rotate the camera so you can see the enemy ships.  Leftclick on one of them to attack it.
#define TUT_9_2_AttackingBandbox				(309 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Good.  To attack them all at once, hold down (CTRL) and drag a selection box around the enemy ships.
#define TUT_9_3_AttackingTimeStop				(310 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	I've paused the action to teach you how to focus the camera without selecting.

/* Lesson 10: TitleAltFocus */
#define TUT_10_ALT_Focusing						(311 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//  Lesson 10: (ALT)-Focusing.  Click Next to begin.
#define TUT_10_1_AltFocus						(312 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Hold down the (ALT) key and leftclick on any ship.  You can even use this to focus on enemy ships.
#define TUT_10_2_AltFocusBandbox				(313 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	You can also focus on multiple ships at once by holding (ALT) and dragging a selection box around them.  Try it now.
#define TUT_10_3_PracticeAndUnPause				(314 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Good.  Now practice focusing until you're ready to move on.  Press (P) to unpause the action.
#define TUT_10_4_BuildFrigate					(315 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The Capital Ship Drive research you started earlier has been completed.  You can now build an Assault Frigate.  Go into the Build Manager.
#define TUT_10_5_BuildFrigateHintRetard			(316 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Try using the taskbar.
#define TUT_10_6_BuildFrigateStart				(317 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Build an Assault Frigate.
#define TUT_10_7_BuildFrigateUnderway			(318 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Let's return to your Scouts.  Leftclick on the Close button.

/* Lesson 11: TitleDock */
#define TUT_11_Docking							(319 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//  Lesson 11: Docking.  Click Next to begin.
#define TUT_11_0_Docking						(320 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Some of your scouts might be damaged or low on fuel.  To dock them with the Mothership, first bring up the rightclick menu.
#define TUT_11_1_SelectDock						(321 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Now select Dock.
#define TUT_11_2_ScoutsDocking					(322 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	The Scouts are on their way to the Mothership to be repaired and refueled.

/* Lesson 12: Cancelling Commands */
#define TUT_12_CancellingOrders					(323 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//  Lesson 12: Cancelling Orders.  Click Next to begin.
#define TUT_12_0_CommandCancelIntro				(324 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	You've learned how to give orders.  Now let's learn how to cancel one.
#define TUT_12_1_CommandCancelContextMenu		(325 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Rightclick on the Scouts and bring up their menu.
#define TUT_12_2_CancelCommand					(326 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Select cancel.
#define TUT_12_3_CancelCommandDescription		(327 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Notice that your Scouts have stopped.  We actually do want them to dock, so issue the dock order again.
#define TUT_12_4_TutorialPractice				(328 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	I have a feeling a battle is imminent.  When your Frigate is built, I bet you will be attacked by a small group of Corvettes and Scouts.
#define TUT_12_7_TutorialFinish					(329 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	Congratulations! You have now finished the Homeworld tutorial.


/******************************************
****                                   ****
****          Single Player            ****
****                                   ****
******************************************/

/******************************************
****            Mission 1              ****
******************************************/

/* 0.3 ANIMATIC - Opening */
#define A00_Narrator_Opening					(330 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	100 years ago, a satellite detected an object under the sands of the Great Desert.
#define A00_Narrator_Opening2					(331 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	An expedition was sent.
#define A00_Narrator_Opening3					(332 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	An ancient starship, buried in the sand.
#define A00_Narrator_Opening4					(333 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	Deep inside the ruin was a single stone that would change the course of our history forever.
#define A00_Narrator_Opening5					(334 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	On the stone was etched a galactic map ...
#define A00_Narrator_Opening6					(335 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	and a single word more ancient than the clans themselves ...
#define A00_Narrator_Opening7					(336 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	Higaara.
#define A00_Narrator_Opening8					(337 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	Our home.
#define A00_Narrator_Opening9					(338 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	The clans were united and a massive colony ship was designed.
#define A00_Narrator_Opening10					(339 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	Construction would take 60 years.
#define A00_Narrator_Opening11   				(340 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	It would demand new technologies, new industries and new sacrifices.
#define A00_Narrator_Opening12   				(341 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	The greatest of these was made by the scientist Karan Sajet who had herself permanently integrated into the colony ship as its living core.
#define A00_Narrator_Opening13   				(342 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	She is now Fleet Command.
#define A00_Narrator_Opening14   				(343 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	The promise of the Guidestone united the entire population.
#define A00_Narrator_Opening15   				(344 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	Every mind became focused on the true origin of our people...
#define A00_Narrator_Opening16   				(345 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	...every effort on the construction of the ship that would seek it out among the stars.

/* 0.5 N01 - Opening (Planet of Exile System) - EZ01 */
#define N01_All_Misc_1							(346 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	What a beautiful sight
#define N01_All_Misc_2							(347 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	You are cleared to approach
#define N01_All_Misc_3							(348 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Bays five, six and seven sealed.
#define N01_All_Misc_4							(349 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Release crews standing by.
#define N01_All_Misc_6							(350 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Bays eight, nine and ten sealed.
#define N01_All_Misc_7							(351 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Scaffold decks A, B, C secure.
#define N01_All_Misc_8							(352 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	You got it
#define N01_All_Misc_9							(353 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Roger that Control.
#define N01_All_Misc_10							(354 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	…decks D and E secure. Scaffold secure.
#define N01_All_Misc_11							(355 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//  Scaffold control standing by.
#define N01_All_Misc_12							(356 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	All systems green.
#define N01_All_Misc_13							(357 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Forty-two seven-oh-one please confirm.
#define N01_All_Misc_14							(358 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//  We're ready.
#define N01_All_Misc_15							(359 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	How do we look? All right, take it to one-fifteen. Locked in.
#define N01_Fleet_Intro							(360 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	This is Fleet Command.  Reporting Mothership pre-launch status.
#define N01_Fleet_Command						(361 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Command on-line...
#define N01_Fleet_Resourcing					(362 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Resourcing online...
#define N01_Fleet_Construction					(363 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Construction online...
#define N01_Fleet_Cryo_AtoJ						(364 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Cryogenic subsections A through J online...
#define N01_Fleet_Ctyo_KtoS						(365 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	K through S online...
#define N01_Fleet_Hull							(366 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Hull pressure stable.
#define N01_Fleet_ScaffoldAlign					(367 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Scaffold Control stand by for alignment...
#define N01_Fleet_AlignConfirmed				(368 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Alingment confirmed.  Stand by Release Sync Control.
#define N01_All_CaliperReleased					(369 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	All caliper banks released...
#define N01_All_DownTheMiddle					(370 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	...she's right down the middle.
#define N01_All_FleetClear						(371 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Initial Fleet is clear.
#define N01_Fleet_MoshipClear					(372 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	The Mothership has cleared the Scaffold.
#define N01_Fleet_MoshipAway					(373 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	We are away.
#define N01_All_CommandLineGreen				(374 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Command Line green.  Initial Fleet in position.
#define N01_Chatter_Mid							(375 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	for NIS's, add to the atmosphere, relatively calm
							

/* 1 M01 - TRIALS (KHARAK SYSTEM) - EZ01 */
#define M01_Intel_GoingOnline					(376 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Fleet Intelligence going on-line.  Our task is to analyze all sensor data and generate mission objectives.  Before the hyperdrive test, several trials must be completed.
#define M01_Intel_ResearchObjective				(377 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Test construction by building the primary research ship.
#define M01_Intel_HavestingObjective			(378 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Test resource processing by harvesting the asteroids provided nearby.
#define M01_Intel_CombatTrials					(379 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Standby to begin combat trials.  First we will be monitoring formation performance.
#define M01_Intel_CombatTrialsObjective			(380 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Target Drones have been provided here.  Assign a formation to your Fighters and destroy the Drones.
#define M01_Intel_CombatUseFormation			(381 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Target Drones destroyed.  Replacement Drones are being sent to the same location.  Completion of this trial requires the use of a formation.  Begin again.
#define M01_Intel_FormationAnalysis				(382 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Formation trial complete.  Flight analysis shows a twenty-two percent increase in combat performance.
#define M01_Intel_TacticsTrial					(383 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  The next trial will test the effectiveness of tactics.  Standby to begin tactics trial.  Use Aggressive or Evasive tactics and engage the Target Drones here.
#define M01_Intel_CombatUseTactics				(384 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Target Drones destroyed.  Replacement Drones are being sent to the same location.  Completion of this trial requires the use of tactics.  Begin again.
#define M01_Intel_TacticsComplete				(385 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Tactics trial complete.
#define M01_Intel_SalvageTrial					(386 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  The next trial will test the performance of the Salvage Corvette.  Build one and capture the Target Drone here.
#define M01_Intel_SalvageTargetDestroyed		(387 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Target Drone destroyed.  A replacement Drone is being sent to the same location.  Completion of this trial requires a successful capture of the drone.  Begin again.
#define M01_Intel_BuildInterceptors				(388 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Interceptors are ready for construction.
#define M01_Intel_AdvancedCombatTrials			(389 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Advanced Target Drones with heavier armor have been provided.  Using Interceptors, begin the mock combat exercise here.
#define M01_Intel_CombatUseInterceptors			(390 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Target Drones destroyed.  Replacement Drones are being sent to the same location.  Completion of this trial requires the use of Interceptors.  Begin again.
#define M01_Fleet_HyperspaceCharging			(391 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace module charging.  35% capacity and rising.  The Mothership will be ready for the Hyperdrive test in 10 minutes.
#define M01_Intel_TrialsComplete				(392 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Standby for Hyperdrive test.  Internal pressure doors will be sealed in 2 minutes.  Abort systems standing by.
#define M01_Fleet_HyperspaceReady				(393 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace module fully charged.  I am ready to initiate quantum wave generation on your mark.  Good luck everyone.
#define M01_Intel_HyperspaceReady				(394 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	All sections have reported in.  Trigger the hyperspace drive at your discretion.

/* 1.19 ANIMATIC - M01à M02 */
#define A01_Fleet_HyperspaceFullPower			(395 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	All Hyperspace systems operating at full power.
#define A01_Intel_TargetingSystem				(396 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	If the hyperspace targeting system is accurate, we should emerge in close proximity to the support vessel Khar-Selim.
#define A01_Intel_2								(397 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	This ship has spent the past 10 years travelling on conventional drives to reach the edge of the Kharak system.
#define A01_Intel_3								(398 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	The Khar-Selim will monitor the quantum waveform as we return to normal space and assist in tuning our drive control systems.
#define A01_Intel_4								(399 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	If the hyperspace module malfunctions, the Khar-Selim will offer assistance and re-supply.
#define A01_Intel_5								(400 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Mission objectives will be to dock with the service vessel and link up research teams in order to complete adjustments to the Mothership and her drives.


/******************************************
****            Mission 2              ****
******************************************/

/* 2 M02 - BATTLE W/ TURANIC RAIDERS (GREAT WASTELANDS) - EZ02 */
#define M02_Fleet_MadeIt						(401 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	We made it.  Hyperspace jump complete!  All systems nominal and the quantum wave effect has dissipated.
#define M02_Intel_MissingScienceVessel			(402 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We have miss-jumped.  The support ship is not here.  Fleet Command will signal the Khar-Selim while we confirm our true position.
#define M02_Fleet_CallingScienceVessel			(403 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	This is the Mothership calling Support Vessel Khar-Selim.  Come in please.  We have miss-jumped and are requesting your beacon. . .  This is the Mothership calling Support Vessel Khar-Selim.  Please Respond. . .
#define M02_Intel_PickedUpBeacon				(404 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Priority Alert.  We have picked up an automated beacon from the Khar-Selim.
#define M02_Intel_SendProbe						(405 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Send a probe to make contact and re-establish communications.
#define M02_Intel_ScienceVesselDestroyed		(406 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Khar-Selim has been destroyed.  Heavy weapon damage is visible on the remaining fragment.  A Salvage Corvette must be sent to retrieve the mission data recorder.
#define M02_Fleet_MothershipUnderAttack			(407 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	They're attacking?!. . . I'm under attack!  The Mothership is under attack!!
#define M02_Intel_DefendMothership				(408 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Engage incoming units.  The Mothership must be defended.
#define M02_Intel_EnemyDestroyed				(409 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Hostile units destroyed.  Threat eliminated.
#define M02_Fleet_MinorDamage					(410 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Mothership sustained minor hull damage.  Repairs are underway.
#define M02_Intel_MoreEnemies					(411 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Priority Alert!  Additional hostile units detected on an intercept course with the Khar-Selim.
#define M02_Intel_ProtectSalvageTeam			(412 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Protection of the Salvage team is a primary Objective.  It must return to Mothership with the mission recorder.  We need that data.
#define M02_Intel_SalCorDestroyed				(413 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Salvage Corvette has been destroyed.  Use an escort force to guard next team.  We need those records.
#define M02_Intel_SalvageDocked					(414 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Salvage Team docked safely.  Downloading Mission Data Recording.  Replaying last entry:
#define M02_All_ScienceVesselAttacked			(415 + SPEECH_ACTOR_KHAR_SELIM + SPEECH_TYPE_SINGLE_PLAYER)	//	What do you mean you detect a Hyperspace entry?!  The mothership isn't due for-...
#define M02_10_Intel_DefendMoShipAgain			(416 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We are detecting enemy units advancing on our position.  Organize a defense force to protect the Mothership.
#define M02_11_Intel_InvestigatePowerSource		(417 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Large power signature detected nearby. Recommend immediate investigation.
#define M02_12_Intel_EnemyCarrier				(418 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	It's an enemy Carrier.  They appear to be reinforcing their squadrons of Fighters and Corvettes with it.
#define M02_Intel_EnemyInferior					(419 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  We have determined that these enemy units are inferior to ours.
#define M02_13_Intel_DestroyAllP1				(420 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Commander, this race is clearly hostile. We can't allow them to secure a position in our system. I recommend that we destroy the force completely.
#define M02_Intel_ObjectivesComplete			(421 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Enemy units are retreating.  Objectives complete.  Standby for immediate return to Kharak.

/* 2.15 ANIMATIC - M02à M03 */
#define A02_Intel_AnalysisOfWreckage			(422 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Analysis of wreckage reveals the hostile units are using strike craft ranging from Fighters to combat Corvettes.
#define A02_Intel_2								(423 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	All pilots will be briefed in case hostiles have penetrated further into the Kharak system.
#define A02_Intel_3								(424 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	On our return to Kharak, the final outfit of the Mothership must be accelerated in order to defend against possible future attacks.
#define A02_Intel_4								(425 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Many major Mothership systems are still incomplete.
#define A02_Intel_5								(426 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We will notify the Kharak Missile Defense System of this possible threat.
#define A02_Intel_6								(427 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	The Mothership will then dock with the Scaffold for repairs.
#define A02_Intel_7								(428 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Standby for Hyperspace exit to Kharak.


/******************************************
****            Mission 3              ****
******************************************/

/* 3 BACK TO KHARAK (KHARAK SYSTEM) - EZ01 */
#define M03_Fleet_ItsGone						(429 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	No one's left. . .Everything's gone!. . .Kharak is burning!
#define M03_Intel_Analyzing						(430 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Kharak is being consumed by a firestorm.  The Scaffold has been destroyed.  All orbital facilities destroyed.  Significant debris ring in low Kharak orbit.  Receiving no communications from anywhere in the system... Not even beacons.
#define M03_Fleet_CryotraySignal				(431 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Wait!  On the maintenance frequency.  I'm getting a signal from the Cryo Tray systems in orbit.  One of them is suffering a massive malfunction.
#define M03_Intel_DispatchSalCorvettes			(432 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Dispatch Salvage Corvettes immediately to collect the trays.
#define M03_Intel_CryotrayUnderAttack			(433 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Cryo Trays are under attack.  These ships are different from those we encountered at the Khar-Selim.  It is likely they were involved in the destruction of Kharak.
#define M03_Intel_CaptureEnemy					(434 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Capture at least one vessel for interrogation and destroy the rest.
#define M03_Fleet_SaveCryotray					(435 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Those trays contain all that remain of our people.  Without them, we will become extinct.
#define M03_Intel_EnemyCaptured					(436 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Hostile vessel captured.  Crew interned.  Interrogation is underway.  While searching the enemy ship's computer systems, we came across these flight recordings.  Standby for playback.

/* 3.5 N04 - Learn Of Sacking (Kharak System) EZ01 */
#define N04_All_Battle1							(437 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Group three moving in... allright now, stay together... ready... SPREAD!
#define N04_All_Battle2							(438 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Surface attack group to first positions...
#define N04_All_Battle3							(439 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Incoming missiles...
#define N04_All_Battle4							(440 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Evasive maneuvers!
#define N04_All_Battle5							(441 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	We're hit! Damage report!
#define N04_All_Battle6							(442 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	What the...?
#define N04_All_Battle7							(443 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	There's another one...
#define N04_All_Battle8							(444 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Prepare for immediate surface bombardment!
#define N04_All_Battle9							(445 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Standing by.
#define N04_All_Battle10						(446 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Commence surface delivery.
#define N04_All_Battle11						(447 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Delivery confirmed.
#define N04_All_Battle12						(448 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	All targets acquired.
#define N04_All_Battle13						(449 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Multiple impacts confirmed.
#define N04_All_Battle14						(450 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Surface temperature fifteen-twenty-three and climbing...
#define N04_All_Battle15						(451 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Estimated immediate casualties: 98 percent.
#define N04_All_Battle16						(452 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Surface temperature seventeen-forty-two and stable...
#define N04_All_Battle17						(453 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Orbital target acquired.
#define N04_All_Battle18						(454 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Watch the spread pattern... stay sharp.
#define N04_All_Battle19						(455 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Target destroyed.
#define N04_All_Battle20						(456 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Requesting clearance to dock.
#define N04_All_Battle21						(457 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Clearance granted.
#define N04_All_Battle22						(458 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Primary directive achieved.
#define N04_All_Battle23						(459 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Prepare for secondary directive.
#define N04_All_Battle24						(460 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Fleet trajectory calculated and locked in.
#define N04_All_Battle25						(461 + SPEECH_ACTOR_ALL_ENEMY_SHIPS + SPEECH_TYPE_NIS)	//	Prepare for hyperspace.

/* 3 BACK TO KHARAK (KHARAK SYSTEM) - EZ01 */
#define M03_Intel_RecordingAnalysis				(462 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	...Analysis of the recording indicates that the Kharak missile defenses heavily damaged the attacking fleet.  However, we have concluded that at present they can still easily defeat us.  We have therefore plotted a course to a deep space asteroid belt.  There we can hide and prepare our fleet for an assault.
#define M03_Intel_ResearchAnalyzedFrigate		(463 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Our research division has analyzed the captured frigate.  We have reverse engineered the drive technology and developed two new ships.  Plans for a third vessel are underway, but will require Frigate Chassis research.
#define M03_Fleet_CryotraysLoaded100			(464 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Cryo tray loaded.  One hundred thousand people secured.
#define M03_Fleet_CryotraysLoaded200			(465 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Cryo tray loaded.  Two hundred thousand people secured.
#define M03_Fleet_CryotraysLoaded300			(466 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Cryo tray loaded.  Three hundred thousand people secured.
#define M03_Fleet_CryotraysLoaded400			(467 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Cryo tray loaded.  Four hundred thousand people secured.
#define M03_Fleet_CryotraysLoaded500			(468 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Cryo tray loaded.  Five hundred thousand people secured.
#define M03_Intel_DeploySalvageTeam				(469 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Withdraw attack and deploy a Salvage Team.  We need that ship.
#define M03_Intel_HyperspaceReady				(470 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Cryo trays loaded and secure.  Hyperspace module charged.  There's nothing left for us here.  Let's go.

/* 3.17 ANIMATIC - M03à M04 */
#define A03_Fleet_CryotraysProcessed			(471 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	Cryogenic trays have been processed and our colonists are safe for now.
#define A03_Fleet_2								(472 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	We are all that's left of our world, our culture, our people.
#define A03_Fleet_3								(473 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	No one survived. . .
#define A03_Intel_InterrogationInfo				(474 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	From the interrogation we learned that a frontier fleet patrolling the borders of a vast interstellar empire was dispatched to destroy our planet.
#define A03_Intel_2								(475 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	The captain claimed our people violated a 4000 year old treaty forbidding us to develop hyperspace technology.
#define A03_Intel_3								(476 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Extermination of our planet was the consequence.
#define A03_Intel_4								(477 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	The subject did not survive interrogation.


/******************************************
****            Mission 4              ****
******************************************/

/* 4 M04 - DEFEAT P1 (GREAT WASTELANDS) EZ04 */
#define M04_Fleet_HyperspaceSuccesful			(478 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//  Hyperspace jump successful.
#define M04_Intel_ScannerData					(479 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Scanner data indicates asteroid density is highest in this region.  Commence resource gathering.
#define M04_Intel_ResearchController			(480 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Resource Controllers are available for construction.  To improve the efficiency of collection, build one and position it near the asteroid vein.
#define M04_Intel_LongRangeSensors				(481 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Long range sensors indicate a Mothership-class mass signature.  It's coming in fast.  Power readings are off the scale.  Full combat alert.  Standby for contact.

/* 4.5 N03 - Traders Intro (Great Wastelands) - EZ03 */
#define N03_Intel_AnomalyDetected				(482 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_NIS)	//	Prepare the ambassador.
#define N03_Fleet_AmbassadorAway				(483 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Ambassador away.
#define N03_All_LockedIn						(484 + SPEECH_ACTOR_AMBASSADOR + SPEECH_TYPE_NIS)	//	Trajectory locked in, hailing signal open on all channels.
#define N03_All_MagneticField					(485 + SPEECH_ACTOR_AMBASSADOR + SPEECH_TYPE_NIS)	//	Entering magnetic field now... almost there...
#define N03_All_LostGuidance					(486 + SPEECH_ACTOR_AMBASSADOR + SPEECH_TYPE_NIS)	//	Fleet, we've lost Guidance and are being drawn in...
#define N03_All_LotsOfLights					(487 + SPEECH_ACTOR_AMBASSADOR + SPEECH_TYPE_NIS)	//	There's a lot of lights... uh... there seems to be... some kind of activity inside, I can see...
#define N03_Traders_Intro						(488 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	The whole big speel here
#define N03_All_AmbassadorReturning				(489 + SPEECH_ACTOR_AMBASSADOR + SPEECH_TYPE_NIS)	//	Fleet, this is ambassador.  We are clear of the Bentusi vessel, all systems green.  Harbor control has released Guidance and the exchange unit is secure.  Receiving crews, prep the quarantine chamber.
#define N03_All_QuarantineReady					(490 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//	Roger that.  Quarantine standing by.
#define N03_Fleet_TradingEstablished			(491 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Bentusi Trading link established.

/* 4 M04 - DEFEAT P1 (GREAT WASTELANDS) EZ04 */
#define M04_Traders_FarewellKushan				(492 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	Turanic Raiders, servants of the Taiidan, are arriving.  They must not learn of our contact.  We must depart.  All that moves is easily heard in the void.  We will listen for you.  Farewell.
#define M04_Traders_FarewellTaiidan				(493 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	Turanic Raiders, servants of the Kushan, are arriving.  They must not learn of our contact.  We must depart.  All that moves is easily heard in the void.  We will listen for you.  Farewell.
#define M04_Traders_StopAttackNow				(494 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	The Bentusi wish only to trade and make contact.  Your attack is unwarranted and ill-advised.  Stop now.
#define M04_Intel_CeaseFire						(495 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Recommend immediate cease-fire.
#define M04_Traders_KickAss						(496 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	You insist on conflict.  This is most unfortunate.
#define M04_Intel_DefendTheFleet				(497 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Turanic Raider fighters are attacking our resource operation.  Defend it and prepare for more hostile ships to arrive.
#define M04_Intel_DefendTheMothership			(498 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Several Turanic Raider capital ships have just emerged from hyperspace near the Mothership.
#define M04_Intel_DiscoveredCarrier				(499 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Turanic Raider Carrier has been located.  It is reinforcing their squadrons of Fighters and Corvettes.
#define M04_Intel_CarrierRetreatingKushan		(500 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Turanic Raider Carrier is retreating.  If it escapes they will warn the Taiidan fleet of our pursuit.  Do not allow them to hyperspace.
#define M04_Intel_CarrierRetreatingTaiidan		(501 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Turanic Raider Carrier is retreating.  If it escapes they will warn the Kushan fleet of our pursuit.  Do not allow them to hyperspace.
#define M04_Intel_HyperspaceKushan				(502 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  The Turanic Raiders have been defeated.  We can now return to our immediate goal: preparation for an attack on the Taiidan fleet that devastated Kharak.
													//	Using data from the Taiidan vessel captured at Kharak, we have been able to determine their location.  If we strike now we can take advantage of their damaged condition.
													//	Hyperspace co-ordinates have been transferred to Fleet Command.
#define M04_Intel_HyperspaceTaiidan				(503 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  The Turanic Raiders have been defeated.  We can now return to our immediate goal: preparation for an attack on the Kushan fleet that devastated Kharak.
													//	Using data from the Kushan vessel captured at Kharak, we have been able to determine their location.  If we strike now we can take advantage of their damaged condition.
													//	Hyperspace co-ordinates have been transferred to Fleet Command.

/* 4.12 ANIMATIC - M04à M05 */
#define A04_Intel_SystemsOptimal				(504 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We have repaired the damage incurred by the Turanic Raiders.
#define A04_Intel_SystemsOptimal2				(505 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Combat and sensor systems returning to optimal functionality.
#define A04_Fleet_Whining						(506 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	I can't believe this happened.
#define A04_Fleet_Whining2						(507 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	Our first hyperdrive test led to near genocide.
#define A04_Fleet_Whining3						(508 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	Kharak... destroyed.
#define A04_Fleet_Whining4						(509 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	All of us, all that's left, hunted by two alien races...
#define A04_Intel_ApproachingCoordinates		(510 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We must focus on the matter at hand: elimination of the fleet that destroyed our world.
#define A04_Intel_ApproachingCoordinates2		(511 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Coming up on hyperspace co-ordinates.


/******************************************
****            Mission 5              ****
******************************************/

/* 5 M05 - BATTLE AGAINST SACKING FORCE (GARDEN VIEW) EZ05 */
#define M05_Fleet_HyperspaceSuccessful			(512 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace successful.  Current position and pre-jump coordinates are in perfect alignment.  We are on target.
#define M05_Intel_SendProbesKushan				(513 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We detect large resource deposits but no vessels.  It is possible that the Taiidan could be hiding in the denser portions of the belt, which may cause interference with our sensors.
#define DummyIGNORE								514	//	Send probes to investigate.  All fleet assets should be kept on alert.
#define M05_Intel_SendProbesTaiidan				(515 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We detect large resource deposits but no vessels.  It is possible that the Kushan could be hiding in the denser portions of the belt, which may cause interference with our sensors.
#define DummyIGNORETOO							516	//	Send probes to investigate.  All fleet assets should be kept on alert.
#define M05_Intel_ProfilesMatch					(517 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Ship profiles and markings match those from the recording at Kharak.  There is no doubt that this is the fleet.  Destroy them.
#define M05_Intel_ResearchIonCannon				(518 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Research Division reports it is now equipped for Ion Cannon technology.  We advise commencing research immediately.
#define M05_Intel_ResearchPlasmaBomb			(519 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Plasma Bomb technology.  We advise commencing research immediately.
#define M05_Intel_DestroyEnemyCollector			(520 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Sensors indicate the enemy is harvesting resources.  Destroy their collector to impair ship production.
#define M05_Intel_MoveAttack					(521 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Enemy capital ships appear to be most heavily armored on the front and sides.  Our capital ships should be issued move orders while attacking to take advantage of the more vulnerable sides.
#define M05_Intel_ResearchDefenseFighter		(522 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  The enemy is using a new Fighter class ship with strong defensive capabilities but low maneuverability.  Our Research Division reports that it can produce a similar vessel.  Begin Research as soon as possible.
#define M05_Intel_HyperspaceKushan				(523 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Taiidan fleet destroyed.  There's nothing left for us to return to.  Our only option is to follow the path etched into the Guidestone.  Finding our ancient home is our only hope now.Hyperspace coordinates locked in.	
#define M05_Intel_HyperspaceTaiidan				(524 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Kushan fleet destroyed.  There's nothing left for us to return to.  Our only option is to follow the path etched into the Guidestone.  Finding our ancient home is our only hope now.Hyperspace coordinates locked in.


/* 5.9 ANIMATIC - M05à M06 */
#define A05_Intel_1								(525 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We've completed de-crypting data from the enemy frigate we captured in the Kharak system.
#define A05_Intel_2								(526 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	It appears to be an imperial broadcast.
#define A05_Intel_3								(527 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	In order to stay clear of these outposts, our course will take us into a turbulent asteroid field and through the heart of a nebula.


/******************************************
****            Mission 6              ****
******************************************/

/* 6 M06 - ASTEROIDS 3D (DIAMOND SHOALS) EZ06 */
#define M06_Fleet_HyperspaceSuccessful			(528 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace jump successful.
#define M06_Intel_AsteroidCollision				(529 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We haven't cleared the asteroid field.  Prepare for collisions.
#define M06_Intel_DestroyAsteroids				(530 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Incoming asteroids must be destroyed before they impact with the Mothership.  Concentrate fire within this collision envelope.
#define M06_Intel_ResearchSuperCap				(531 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Super Capital Ship Drive technology.  We advise commencing research immediately.
#define M06_Intel_StrikeCraftIneffective		(532 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Strike Craft are proving ineffective.  We recommend using primarily capital ships.
#define M06_Intel_ClearedField					(533 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We've cleared the field.
#define M06_Traders_ComeToTrade					(534 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	Greetings.  We have come to trade.
#define M06_Fleet_AskTradersForInfo				(535 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	This is a dangerous and unpredictable region.  Can you give us information that will guide us through the nebula ahead?
#define M06_Traders_FearNebulaKushan			(536 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	We hear nothing there.  Even the Taiidan fear the Great Nebula.  No one returns.
#define M06_Traders_FearNebulaTaiidan			(537 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	We hear nothing there.  Even the Kushan fear the Great Nebula.  No one returns.
#define M06_Intel_HyperspaceReady				(538 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace Module fully charged.

/* 6.11 ANIMATIC - M06à M07 */
#define A06_Fleet								(539 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	The Bentusi said "No one returns."


/******************************************
****            Mission 7              ****
******************************************/

/* 7 M07 - SWARMER BATTLE (GARDENS) EZ07 */
#define M07_Intel_HarvestNebula					(540 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Nebula is incredibly rich in energy and resources.  Energy levels are so high that our sensors are having trouble compensating.  Begin harvesting the nebula while we address this problem.
#define M07_Intel_AlertStatus					(541 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	There is a contact closing with the Mothership.  Sensors instability in this region makes it difficult to identify.
#define M07_Intel_PrepareAmbassador				(542 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Prepare the Ambassador.

/* 7.3 N05 - P2 Intro (Gardens) EZ07 */
#define N05_Fleet_AmbassadorAway				(543 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)	//	Ambassador away.
#define N05_P2_Intro1							(544 + SPEECH_ACTOR_P2_KADESHI + SPEECH_TYPE_NIS)	//	This is the Garden of Kadesh.  For thirteen generations we have protected it from the unclean.
#define N05_P2_Intro2							(545 + SPEECH_ACTOR_P2_KADESHI + SPEECH_TYPE_NIS)	//	The Turanic Raiders who came before you failed to join and were punished for this trespass. Like theirs, your ship has already defiled this holy place.
#define N05_P2_Intro3							(546 + SPEECH_ACTOR_P2_KADESHI + SPEECH_TYPE_NIS)	//	If you have come to join we welcome you and will spare your ship until all have disembarked.  If you have come to consume the garden you will be removed at once.
#define N05_P2_Intro4							(547 + SPEECH_ACTOR_P2_KADESHI + SPEECH_TYPE_NIS)	//	What are your intentions?
#define N05_All_UnawareOfSignificance			(548 + SPEECH_ACTOR_AMBASSADOR + SPEECH_TYPE_NIS)	//	We were unaware of the significance of this location.  We mean you no conflict.  Please allow us time to prepare our engines so that we may withdraw as requested.
#define N05_P2_NotARequest						(549 + SPEECH_ACTOR_P2_KADESHI + SPEECH_TYPE_NIS)	//	If you will not join, then die.  There is no withdrawal from the Garden.

/* 7 M07 - SWARMER BATTLE (GARDENS) EZ07 */
#define M07_Intel_DefendMothership				(550 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Delay the attacking ships while Fleet Command charges the hyperdrive module.  We should have the range to jump clear of the nebula.
#define M07_Fleet_HyperspaceCharging1			(551 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace module at 35%, ready in 8 minutes.
#define M07_Fleet_HyperspaceCharging2			(552 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace module at 90%, ready in 1 minute.
#define M07_Fleet_HyperspaceReady				(553 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace module fully charged.  Get us out of this place.
#define M07_Intel_EngageHyperdrive				(554 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Co-ordinates set.  Engage hyperdrive!
#define M07_Fleet_HyperdriveFailed				(555 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperdrive jump failed!  The Quantum waveform collapsed before the ship could enter hyperspace.
#define M07_Intel_AnalyzingMalfunction			(556 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Analyzing malfunction.  Continue to protect the Mothership until the problem is solved.
#define M07_Intel_DefendCollectors				(557 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Enemy forces are concentrating on our Resource Collectors.  Allocate combat vessels to protect them.
#define M07_Fleet_HypdriveOnline				(558 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	The Hyperdrive is back on-line.
#define M07_Intel_Hyperspace					(559 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Let's put these fanatics behind us.

/* 7.12 ANIMATIC - M07à M08 */
#define A07_Intel_1								(560 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	The enemy Mothership appeared to be equipped with a powerful field generator.
#define A07_Intel_2								(561 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	This field deformed our quantum wavefront and prevented us from making a hyperspace jump.
#define A07_Intel_3								(562 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We also observed that the enemy's hyperspace module has an identical power signature to our own.
#define A07_Intel_4								(563 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	This raises interesting questions considering our own technology was reverse-engineered from the wreck of the Khar-Toba.
#define A07_Intel_5								(564 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Our hyperspace systems are now functioning properly and this jump will carry us clear of the Nebula.


/******************************************
****            Mission 8              ****
******************************************/

/*8 M08 - FALKIRK (OUTER GARDENS) EZ08 */
#define M08_Fleet_HyperspaceInterrupted			(565 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Something's wrong.  We've been pulled out of hyperspace.  We're still inside the nebula.
#define M08_Intel_ItsATrap						(566 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	It's a trap!
#define M08_Intel_DestroyInhibitors				(567 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Sensors detect hyperspace inhibitors in a triangular formation.  Even one can keep us from entering hyperspace.  All of them must be destroyed.
#define M08_Intel_DestroyHostiles				(568 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The nebula is still scrambling our sensors but it looks like we have incoming hostiles.
#define M08_Intel_ResearchDrones				(569 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Research Division reports it is now equipped for Drone technology.  We advise commencing research immediately.
#define M08_Intel_ResearchDefenseField			(570 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Defense Field technology.  We advise commencing research immediately.
#define M08_P2_JoinUs							(571 + SPEECH_ACTOR_P2_KADESHI + SPEECH_TYPE_SINGLE_PLAYER)	//	Again we offer you the chance to join us and live here in peace.
#define M08_Fleet_SomethingInCommon				(572 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Our future lies elsewhere, but we already have something in common.  The hyperdrive technology we use is identical to yours.  Our ancestors left it in a wreckage on our planet.  We're on a mission to find our Homeworld.
#define M08_P2_YouWillFail						(573 + SPEECH_ACTOR_P2_KADESHI + SPEECH_TYPE_SINGLE_PLAYER)	//	You will fail.  The evil that drove us here will find and destroy you.  From you they will know of us and come here.  This cannot come to pass.
#define M08_Intel_DestroyAttackers				(574 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We have enemy units closing on multiple attack vectors.  Engage and destroy hostiles.
#define M08_Intel_EnemyRetreating				(575 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Enemy vessels retreating to this point.  This reading has been consistent despite sensor interference.  It has a friendly signature but it's not one of ours.
#define M08_Fleet_LooksJustLike					(576 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	It looks just like the Khar-Toba.
#define M08_Intel_IdenticalMatch				(577 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Metallurgy and structural composition are an identical match to the Khar-Toba wreckage on Kharak.
#define M08_Fleet_WeAreBrothers					(578 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	We are brothers.
#define M08_Intel_HyperspaceOnline				(579 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The inhibitor field has ceased.  Hyperspace module back on-line.

/* 8.10 ANIMATIC - M08à M09 */
#define A08_Intel_1								(580 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	After analyzing the data we collected from the Khar-Toba's sister ship we've been able to determine what happened in the nebula.
#define A08_Intel_2								(581 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	While the Khar-Toba was able to limp to Kharak, this ship must have tried to hide here in the nebula.
#define A08_Intel_3								(582 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	They soon resorted to preying on ships passing through the nebula.
#define A08_Intel_4								(583 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	In time the nebula became off limits to all shipping.
#define A08_Intel_5								(584 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	They developed hyperspace inhibitor technology to trap prey from far away without leaving the safety of the nebula.
#define A08_Intel_6								(585 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Eventually it became the center of their existence.
#define A08_Intel_7								(586 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Their religion.


/******************************************
****            Mission 9              ****
******************************************/

/* 9 M09 - GHOSTSHIP (WHISPERING OCEAN) EZ09 */
#define M09_Fleet_AnomolyDetected				(587 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Anomaly detected.  Override engaged.
#define M09_Intel_VesselDetected				(588 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Sensors detect a vessel here.  It doesn't match any of the profiles we have encountered.  Send in a team to investigate.
#define M09_Intel_ShipsInactive					(589 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We are detecting various ships surrounding the alien vessel.  They appear to be inactive.
#define M09_Intel_NeutralizeVessel				(590 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Attention!  Those ships are operational.  We believe the control center is the alien vessel.  It should be neutralized.
#define M09_Intel_LostControl					(591 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We've lost control of capital ships in close proximity to the alien vessel.
#define M09_Intel_ControlField					(592 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We've determined that the alien control field covers this area.  No capital ships should cross into this zone or we will lose them.
#define M09_Intel_MinimalEffect					(593 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Our weapons are having minimal effect on the alien vessel but each strike causes a tiny fluctuation in the control field.
#define M09_Intel_DirectAllFire					(594 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Direct all fire at the alien ship in an attempt to disable the field.
#define M09_Intel_ConstructMissleDestroyer		(595 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	After seizing the Missile Destroyer, construction reports we can now build a similar vessel.
#define M09_Intel_VesselNeutralized				(596 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Alien vessel neutralized.  Our crews have regained control.
#define M09_Intel_SalvageUnknownShip			(597 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	While the field was up they were able to analyze the alien control system.  We now have control of the foreign vessels.  In addition, we detect no life signs aboard the alien vessel.  It is a derelict.
#define M09_Intel_ResearchGravWell				(598 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it has developed plans for gravity warping technology based on the alien control field.  We advise commencing research immediately.
#define M09_Traders_CouldNotApproach			(599 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	We have known of this ship but could never approach it.  We are particularly vulnerable to its influence.
#define M09_Traders_ExchangeInfo				(600 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	The Bentusi would like the information you have acquired.  It will be transferred automatically if you choose to trade.
#define M09_Traders_InfoTransfered				(601 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	The information was successfully transferred.  Thank you.
#define M09_Fleet_HelpUsDefeatKushan			(602 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	The Taiidan are determined to destroy us.  Will you help us defeat them?
#define M09_Fleet_HelpUsDefeatTaiidan			(603 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	The Kushan are determined to destroy us.  Will you help us defeat them?
#define M09_Traders_CouncilKushan				(604 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	Conflict is not our way.  We will bring your cause to the Galactic Council.  The Taiidan rule the Empire but even they must answer to the council.
#define M09_Traders_CouncilTaiidan				(605 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	Conflict is not our way.  We will bring your cause to the Galactic Council.  The Kushan rule the Empire but even they must answer to the council.
#define M09_Intel_HyperspaceReady				(606 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace Module fully charged.  Engage at your discretion.

/* 9.14 ANIMATIC - M09à M10 */
#define A09_Intel_Kushan						(607 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We are about to enter the outer limits of the Taiidan empire.
#define A09_Intel_Taiidan						(608 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We are about to enter the outer limits of the Kushan empire.
#define A09_Intel_2								(609 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	As we approach the galactic core, resistance is expected to increase.
#define A09_Intel_3								(610 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We have identified a weak point in the enemy defenses.
#define A09_Intel_4								(611 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	There is a remote research station located near an active supernova.
#define A09_Intel_5								(612 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	It should only have a minor garrison protecting it.
#define A09_Intel_6								(613 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	It is time to hunt the enemy as they have hunted us.


/******************************************
****            Mission 10             ****
******************************************/

/* 10 M10 - SUPERNOVA STATION (WHISPERING OCEAN) EZ10 */
#define M10_Fleet_HyperspaceSuccesful			(614 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace jump successful.  We have cleared the Outer Rim dust bank.
#define M10_Fleet_Supernova						(615 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	The supernova is two hundred and fifteen light years away.  It is emitting intense radiation.
#define M10_Intel_StikeCraftVulnerable			(616 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Strike Craft are especially vulnerable to this radiation.  Capital ships will be the most effective due to their heavy armor.
#define M10_Intel_DestroyOutpost				(617 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Research Station is located here.  Assemble a heavy strike force and destroy it.
#define M10_Intel_RadiationHeavier				(618 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Radiation is much heavier than we expected.  Sensors indicate that asteroids may have shielding properties.
#define M10_Intel_AsteroidProtection			(619 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We recommend using the asteroid pockets for protection.
#define M10_Intel_DeployController				(620 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The radiation is disrupting our normal resource collecting operations.  Deploy a Resource Controller in the shielded asteroid pockets.
#define M10_Intel_ResearchProximitySensor		(621 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Proximity Sensor technology.  We advise commencing research immediately.
#define M10_Intel_ResearchMinlayer				(622 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The enemy is using mines.  Research Division reports it can produce a Corvette-class minelaying ship.  We advise commencing research immediately.
#define M10_Intel_DestroyDefenseForce			(623 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We must destroy the garrison protecting the station.  Enemy units cannot be allowed to escape or they may alert the Empire to our presence.
#define M10_Intel_DestroyCarrierKushan			(624 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We are picking up a quantum wave effect.  A Taiidan Carrier is loading ships and powering up.  It must be destroyed before it can hyperspace.
#define M10_Intel_DestroyCarrierTaiidan			(625 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We are picking up a quantum wave effect.  A Kushan Carrier is loading ships and powering up.  It must be destroyed before it can hyperspace.
#define M10_Intel_HyperspaceReady				(626 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Enemy base and fleet destroyed.  Hyperspace drives online.

/* 10.11 ANIMATIC - M10à M11 */
#define A10_Intel_Kushan						(627 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We have intercepted a coded Taiidan transmission:
#define A10_Intel_Taiidan						(628 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We have intercepted a coded Kushan transmission:
#define A10_Admiral_1							(629 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	You have failed to keep the Exiles from penetrating the outer perimeter.  This could be disastrous.  You will find and destroy them immediately.
#define A10_Admiral_2							(630 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Do not fail us again, or the Elite Guard will require a new commander.
#define A10_Admiral_3							(631 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Our spies believe that the Bentusi have interfered in this matter.  They must not be allowed to bring this matter to the Council and gain support for the Exiles.


/******************************************
****            Mission 11             ****
******************************************/

/* 11 M11 - P3 VS. TRADERS (TENHAUSER GATE) EZ11 */
#define M11_Fleet_HyperspaceDisengaged			(632 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperdrive disengaged.  The Bentusi are here.  They're in distress.
#define M11_Intel_DestroyEnemyKushan			(633 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Bentusi must be protected.  Draw the Taiidan fleet away and destroy them.
#define M11_Intel_DestroyEnemyTaiidan			(634 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Bentusi must be protected.  Draw the Kushan fleet away and destroy them.
#define M11_Traders_BetusiInDebt				(635 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	For the first time in memory, the Bentusi are in the debt of another.

/* 11.4 N08 - Awareness (Tenhauser Gate) Ez11 */
#define N08_Traders_ForbiddenInfo				(636 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	It has been forbidden to possess this information for some time.  But after your intervention on our behalf, we feel compelled to share it with you.  Behold:
#define N08_Traders_Awareness1					(637 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	In the First Time, a terrible war brought with it the collapse of your ancient empire.
#define N08_Traders_Awareness2					(638 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	In an effort to sooth relations, the conquerors spared the lives of the defeated.  All survivors were sent into exile.
#define N08_Traders_Awareness3					(639 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	None were permitted to follow or aid the fallen.  All memory of them was to be erased.
#define N08_Traders_Awareness4					(640 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	For generations the convoy moved silently through space.  They endured great difficulties...
#define N08_Traders_Awareness5					(641 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	...imperfect technology...
#define N08_Traders_Awareness6					(642 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	In time, a suitable system to receive them was found.
#define N08_Traders_Awareness7					(643 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	This barren world appeared to be salvation. Their true legacy forgotten, a new vision of destiny had grown out of captivity.
#define N08_Traders_Awareness8					(644 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//	A single artifact survived the journey.  The Guide Stone you now carry.  It was removed from the sacred "Angel Moon" of your Homeworld, a place long since reduced to myth and tale.
#define N08_Traders_Awareness9					(645 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_NIS)	//  Your progress is becoming known among the Inner Limb worlds and elsewhere.  Many cultures have prophesized your return.

/* 11 M11 - P3 VS. TRADERS (TENHAUSER GATE) EZ11 */
#define M11_Traders_ReachHomeworld				(646 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	Reach your Homeworld.  Establish your claim.  We will summon the Council.
#define M11_Intel_EngageHyperdrive				(647 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Engage Hyperdrive.

/* 0.0 ANIMATIC - M11à M12 */
#define A11_Fleet_1								(648 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	Emergency alert!!
#define A11_Fleet_2								(649 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	Hyperdrive Malfunction.
#define A11_Fleet_3								(650 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	The quantum waveform is collapsing.
#define A11_Fleet_4								(651 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	Emergency interrupt engaged.
#define A11_Fleet_5								(652 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	Prepare for immediate return to normal space.
												

/******************************************
****            Mission 12             ****
******************************************/

/* 12 M12 - ELITE GUARD TRAP (GALACTIC CORE) EZ12 */
#define M12_Fleet_HyperspaceDisengaged			(653 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Dropping out of hyperspace.  All systems online.
#define M12_Intel_DestroyGravWell				(654 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  We are caught in a gravity well.  Fighters and Corvettes will be unable to move.  Seek out the source of this field and destroy it.
#define M12_Intel_ResearchMissleDestroyer		(655 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The enemy is using a Missile Destroyer.  Research Division reports it can produce a similar ship.  We advise commencing research immediately.
#define M12_Intel_ResearchCloadkedFighter		(656 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Research Division reports it is now equipped for Cloaked Fighter technology.  We advise commencing research immediately.
#define M12_Intel_ResearchDefenseFighter		(657 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Defense Fighter technology.  We advise commencing research immediately.
#define M12_Intel_UnderAttackKushan				(658 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We are under attack by Taiidan forces.  They are concentrating fire on our immobilized Strike Craft.
#define M12_Intel_UnderAttackTaiidan			(659 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  We are under attack by Kushan forces.  They are concentrating fire on our immobilized Strike Craft.
#define M12_Defector_DefectingKushan			(660 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_SINGLE_PLAYER)	//	Attention Kushan Mothership!  This is Captain Elson of the Taiidan Elite Guard Destroyer Kapella.  We wish to defect and need assistance.  In return we are prepared to help you.  Please respond.
#define M12_Defector_DefectingTaiidan			(661 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_SINGLE_PLAYER)	//	Attention Taiidan Mothership!  This is Captain Elson of the Kushan Elite Guard Destroyer Kapella.  We wish to defect and need assistance.  In return we are prepared to help you.  Please respond.
#define M12_Intel_ProtectDefector				(662 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	This could be a trap but the Kapella is clearly damaged.  Engage the pursuing fleet and draw it away from the defecting ship.
#define M12_Intel_EngageHyperdrive				(663 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The defecting captain has been brought aboard.  Engage hyperspace.

/* 12.8 ANIMATIC - M12à M13 */
#define A12_Defector_1							(664 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  I am Group Captain Elson of the Destroyer Kapella.
#define A12_Defector_Kushan						(665 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  The Taiidan empire has become decadent and corrupt over the centuries.
#define A12_Defector_Taiidan					(666 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  The Kushan empire has become decadent and corrupt over the centuries.
#define A12_Defector_3							(667 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  The use of the forbidden atmosphere-deprivation device on your planet finally triggered the Rebellion.
#define A12_Defector_4							(668 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  Help me get access to the Rebellion's communication network.
#define A12_Defector_5							(669 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  I will show you a way through the defenses surrounding your Homeworld.
#define A12_Defector_6							(670 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  Take me to the ship graveyard at Karos.
#define A12_Defector_7							(671 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  Hidden in a derelict there is a relay I can use with your help.
#define A12_Defector_8							(672 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_ANIMATIC)	//  The Rebellion waits for my sign to move into its next phase.
#define ShanesARealBigDummy						673	// heh heh.. ;^)

/******************************************
****            Mission 13             ****
******************************************/

/* 13 M13 - SHIP GRAVEYARD (SHINING HINTERLANDS) EZ13 */
#define M13_Fleet_HyperspaceDisengaged			(674 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace transition complete.  We have arrived at Karos.
#define M13_Intel_CommunicationRelay			(675 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The communication relay is here.  A Fighter or Corvette must dock with it to establish the link.
#define M13_Intel_CapShipLost					(676 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Attention.  We have lost contact with one of our capital ships.  It's last recorded position is here.
#define M13_Intel_HyperspaceSignature			(677 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace signatures have been found at these locations but we detect no new ships.
#define M13_Intel_FoundMissingShip				(678 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We've located one of the missing ships.  It appears that it can be salvaged and reactivated.
#define M13_Intel_GraveyardDefended				(679 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Graveyard is defended by autoguns.  They will complicate our attempt to reach the communications array.
#define M13_All_CommLinkEstablished				(680 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_SINGLE_PLAYER)	//	I'm in!  Communication link established.  I've docked!  Communication link established.
#define M13_Defector_ResistanceInformedKushan	(681 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_SINGLE_PLAYER)	//	Patching into command node now... The Taiidan resistance has been informed of your actions and are preparing the fleets.  You have our thanks.
#define M13_Defector_ResistanceInformedTaiidan	(682 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_SINGLE_PLAYER)	//	Patching into command node now... The Kushan resistance has been informed of your actions and are preparing the fleets.  You have our thanks.
#define M13_Defector_CoordinatesTransfered		(683 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_SINGLE_PLAYER)	//	The hyperspace coordinates you require have been transferred to your Mothership.  Farewell.
#define M13_Intel_HyperspaceEnabled				(684 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace Enabled.

/* 13.10 ANIMATIC - M13à M14 */
#define A13_Intel_1								(685 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	According to the data we received from Captain Elson, the Homeworld system is surrounded by a network of hyperspace inhibitors.
#define A13_Intel_2								(686 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	The inhibitors are all heavily shielded and do not show up on any sensors.
#define A13_Intel_3								(687 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Elson has provided us with co-ordinates of the most vulnerable inhibitor station.
#define A13_Intel_4								(688 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Our goal is to destroy the station and create our own access point.


/******************************************
****            Mission 14             ****
******************************************/

/* 14 M14 - MINING FACILITY (BRIDGE OF SIGHS) EZ14 */
#define M14_Fleet_HyperspaceDisengaged			(689 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace successful.  We are at the edge of the Homeworld system.
#define M14_Intel_DestroyGenerator				(690 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Elson's information was correct.  This is the field generator.  We must destroy it.
#define M14_Intel_ResearchHeavyGuns				(691 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Heavy Guns technology.  We advise commencing research immediately.
#define M14_Intel_ResearchSensors				(692 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports advancements in sensor fidelity which would allow us to determine the location of enemy ships.  We advise commencing research immediately.
#define M14_Intel_DestroyHyperspaceGates		(693 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The enemy has activated several standing hyperspace gates.  Destroy the gates to prevent enemy reinforcement.
#define M14_Intel_HyperdriveOnline				(694 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The field surrounding the Homeworld system has been shut down.  Hyperdrive on-line.
#define M14_Fleet_TakeUsHome					(695 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Take us home.

/* 14.8 ANIMATIC - M14à M15 */
#define A14_Intel_1								(696 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	We have to assume that the Homeworld's defensive fleet must be alerted to our presence.
#define A14_Intel_2								(697 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	All vessels and crew at maximum readiness.
#define A14_Intel_3								(698 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	Weapons and tracking at 100% efficiency.
#define A14_Intel_4								(699 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	The fleet is ready.
#define A14_Intel_5								(700 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	There can be no retreat now.


/******************************************
****            Mission 15             ****
******************************************/

/* 15 M15 - HEADSHOT (CHAPEL PERILOUS) EZ15 */
#define M15_Fleet_HyperspaceInterrupted			(701 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace interrupted.
#define M15_Intel_AlertedKushan					(702 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Shutting down the inhibitor field has alerted the Taiidan to our presence.
#define M15_Intel_AlertedTaiidan				(703 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Shutting down the inhibitor field has alerted the Kushan to our presence.
#define M15_Intel_DestroyHeadshot				(704 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  We are on a collision course with a very large object.  It appears to have escorts.  It must be destroyed before it impacts the Mothership.
#define M15_Intel_ImpactIn20					(705 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in twenty minutes.
#define M15_Intel_ImpactIn15					(706 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in fifteen minutes.
#define M15_Intel_ImpactIn10					(707 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in ten minutes.
#define M15_Intel_ImpactIn5						(708 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in five minutes.
#define M15_Intel_ImpactIn4						(709 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in four minutes.
#define M15_Intel_ImpactIn3						(710 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in three minutes.
#define M15_Intel_ImpactIn2						(711 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in two minutes.
#define M15_Intel_ImpactIn1						(712 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in one minute.
#define M15_Fleet_StandbyForImpact				(713 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Standby for impact.
#define M15_Intel_HyperspaceReady				(714 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace module charged and ready.

/* 15.14 ANIMATIC - M15à M16 */
#define A15_1									(715 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	What's happening?  OR  The Enemy must have been desperate.
#define A15_2									(716 + SPEECH_ACTOR_EMPEROR + SPEECH_TYPE_ANIMATIC)	//  Karan.  OR  The time we took to break up that attack has allowed them time to reinforce.
#define A15_3									(717 + SPEECH_ACTOR_EMPEROR + SPEECH_TYPE_ANIMATIC)	//  You've taken one step too close to me.  OR  The chemical composition of this system matches that of the Guidestone.
#define A15_4									(718 + SPEECH_ACTOR_EMPEROR + SPEECH_TYPE_ANIMATIC)	//  From here I can touch you.  OR  We're home.


/******************************************
****            Mission 16             ****
******************************************/

/* 16 M16 - FINAL BATTLE (HOMEWORLD SYSTEM) EZ16 */
#define M16_Intel_LostKaran						(719 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We've lost Karan.  Fleet Command is gone.  Emergency biotech teams are working to keep her alive.
#define M16_Intel_CollisionAsteroidKushan		(720 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The collision asteroid must have served its purpose as a delay tactic.  There is a large number of Taiidan ships located here.
#define M16_Intel_CollisionAsteroidTaiidan		(721 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The collision asteroid must have served its purpose as a delay tactic.  There is a large number of Kushan ships located here.
#define M16_Intel_EmperorsShip					(722 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	A Mothership-class vessel is among them.
#define M16_Intel_DestroyAll					(723 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	All of these forces must be destroyed.  Good luck.
#define M16_Intel_EnemyReinforcements			(724 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Enemy reinforcements emerging from hyperspace.
#define M16_Intel_AnotherFleet					(725 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Another fleet is coming out of Hyperspace right on top of us.  We are being overwhelmed!
#define M16_Defector_CaptainElson				(726 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_SINGLE_PLAYER)	//	This is Captain Elson.  We have been battling reinforcement fleets to get here and have lost many ships already.
#define M16_Defector_EmperorKushan				(727 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_SINGLE_PLAYER)	//  The Emperor's flagship is here.  Together we can defeat him and the Taiidan fleet.  I am placing squadrons Cor and Jasah under your command.
#define M16_Defector_EmperorTaiidan				(728 + SPEECH_ACTOR_DEFECTOR + SPEECH_TYPE_SINGLE_PLAYER)	//  The Emperor's flagship is here.  Together we can defeat him and the Kushan fleet.  I am placing squadrons Cor and Jasah under your command.
#define M16_Intel_DestroyFlagship				(729 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Here is the Emperor's flagship.  It must be destroyed.
#define M16_Fleet_BackOnline					(730 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	...Fleet Command back online.  The Emperor is gone.
#define M16_Traders_BroughtTheCouncil			(731 + SPEECH_ACTOR_TRADERS + SPEECH_TYPE_SINGLE_PLAYER)	//	We have brought the Council.  This war is over.

/* 16.10 ANIMATIC - M15à M16 */
#define A16_Fleet_SigningOff					(732 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_ANIMATIC)	//	This is Fleet Command, signing off.

/* Quick Fixes */
#define N01_All_Misc_5							(733 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)	//  All stations green.
#define M01_Intel_ResearchOnline				(734 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The primary Research Ship has been constructed and the Research Division is online.  Begin Fighter Chassis research immediately.
#define M15_Intel_ImpactIn30secs				(735 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in 30 seconds.
#define M15_Intel_ImpactIn10secs				(736 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Impact in 10 seconds.

#define A15_1_Alt								(737 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//	The Enemy must have been desperate.
#define A15_2_Alt								(738 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//  The time we took to break up that attack has allowed them time to reinforce.
#define A15_3_Alt								(739 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//  The chemical composition of this system matches that of the Guidestone.
#define A15_4_Alt								(740 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_ANIMATIC)	//  We're home.

#define STAT_F_CrateFoundResources				(741 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	STATUS_CrateFound-ResourcesRecovered
#define STAT_F_CrateFoundShips					(742 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	STATUS_CrateFound-ShipsRecovered
#define STAT_F_CrateFoundTech					(743 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	STATUS_CrateFound-TechRecovered
#define COMM_F_NoMoreResearchTopic				(744 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	COMMAND_Research-NoMoreResearchTopics
#define STAT_F_RelicPlayerDies					(745 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	STATUS_RelicPlayerDies-EasterEgg
#define COMM_F_RelicAllianceFormed				(746 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	STATUS_RelicAllianceFormed-EasterEgg
#define COMM_F_RelicAllianceBroken				(747 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	STATUS_RelicAllianceBroken-EasterEgg

#define STAT_CloakedShips_Detected				(748 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS)	//	STATUS_CloakedShips-Detected(Report) (ALL SHIPS)
#define N01_Chatter_BG							(749 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_NIS)
#define N01_Fleet_Standby						(750 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_NIS)		//	Stand by for Initial Fleet Command Line testing...


/* ALTERNATIVE VERSIONS FOR ADAM AND ERIN */

#define M02_Intel_ObjectivesCompleteSHORT		(751 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Objectives complete.  Standby for immediate return to Kharak.
#define M09_Intel_VesselNeutralizedSHORT		(752 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Alien vessel neutralized.
#define M12_Intel_UnderAttackKushanSHORT		(753 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We are under attack by Taiidan forces.
#define M12_Intel_UnderAttackTaiidanSHORT		(754 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  We are under attack by Kushan forces.

/* PICKUPS - Fleet Intel Pickups.doc */

#define M01_Intel_HyperdriveTest				(755 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Standby for Hyperdrive test.  Internal pressure doors sealed.  Abort systems standing by.

#define M02_Intel_EnemyInferior2				(756 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We have determined that these enemy units are inferior to ours.
#define M02_Intel_DestroyCompletely				(757 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  To protect against penetration of the Kharak system, destroy the attacking force completely.
#define M02_Intel_EnemyHyperspacing				(758 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The enemy Mothership is hyperspacing.  Recall all Fighters and prepare for return to Kharak.

#define M03_Intel_DefendCryoTrays				(759 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Cryo Trays are under attack.  Defend them.
#define M03_Intel_SalvageCryoTrays				(760 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Enemy units neutralized.  Begin salvaging the cryo trays.

#define M04_Intel_MoveController				(761 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	To increase harvesting efficiency, move your Resource Controller as close to heavy resource areas as possible.

#define M05_Intel_MoveAttackNEW					(762 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Enemy capital ships appear to have lighter armor on the top, bottom, and rear sides.  Our capital ships should be issued move orders while attacking to take advantage of this weakness.

#define M06_Intel_IonCannonTech					(763 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Ion Cannon technology.  We advise commencing research immediately.
#define M06_Intel_PlasmaBombTech				(764 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Plasma Bomb technology.  We advise commencing research immediately.
#define M06_Intel_DetectingBentusi				(765 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Detecting incoming Bentusi vessel from the clearing ahead.

#define M07_Intel_HyperdriveFailed				(766 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperdrive jump failed!  The Quantum waveform collapsed due to some kind of inhibitor field.
#define M07_Intel_AnalyzingField				(767 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Analyzing field.  Continue to protect the Mothership until the source is located.
#define M07_Intel_FieldDisappeared				(768 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The inhibitor field has disappeared.
#define M07_Intel_HyperdriveFunctional			(769 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Hyperdrive is fully functional.
#define M07_Intel_ResearchMultigunVette			(770 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The enemy is relying heavily on Fighter-class ships.  Our Research division reports it can design a new type of Corvette specially suited to combat multiple fighters.  Begin Research as soon as possible.

#define M08_Intel_DestroyHostilesNEW			(771 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We have enemy units closing from all directions.  Engage and destroy hostiles.
#define M08_Intel_InhibitorCeased				(772 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The inhibitor field has ceased.  Hyperspace ability has been restored.
#define M08_Fleet_ItsTheKharToba				(773 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	It's the Khar-Toba.
#define M08_Intel_IdenticalToKharToba			(774 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Metallurgy and structural composition of the ship are an identical match to the Khar-Toba wreckage on Kharak.  Our origins are the same.

#define M09_Intel_SalvageShip					(775 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	While the field was up they were able to analyze the alien control system.  We now have control of the foreign vessels.
#define M09_Intel_SendSalvageTeam				(776 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  Send a Salvage Team to further investigate the alien ship.
#define M09_Intel_VesselNeutralizedSHORTNEW		(777 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Alien vessel neutralized.
#define M09_Intel_SalvageDocked					(778 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Salvage Team docked.  The alien ship is millions of years old.  Its purpose is unclear.
#define M09_Intel_ResearchGravWellNEW			(779 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  However, Research Division reports it has developed plans for gravity warping technology based on composition of the alien hull.  We advise commencing research immediately.

#define M10_Intel_ResearchCloakGen				(780 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Cloaked Generator technology.  We advise commencing research immediately.
#define M10_Intel_UseViensForProtection			(781 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Radiation is much heavier than we expected.  Sensors indicate that these veins of space dust may have shielding properties.  We recommend using the veins for protection.

#define M11_Intel_ResearchSuperHeavy			(782 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Research Division reports it is now equipped for Super Heavy Chassis technology.  We advise commencing research immediately.

#define SP_Intel_PrimaryObjectiveComplete		(783 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Primary objective complete.
#define SP_Intel_SecondaryObjectiveComplete		(784 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Secondary objective complete.
#define SP_Intel_ConsultObjectives				(785 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	A mission objective remains incomplete. Consult objectives.
#define SP_Intel_MissionFailed					(786 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Mission Failed.

/* PICKUPS - Fleet Command Pickups.doc */
#define M05_Fleet_FollowTheGuidestone			(787 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	There's nothing left for us to return to.  Our only option is to follow the path etched into the Guidestone.  Finding our ancient home is our only hope now.

#define M07_Fleet_Charging8Minutes				(788 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace module charging, ready in 8 minutes.
#define M07_Fleet_Charging1Minute				(789 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperspace module charging, ready in 1 minute.
#define M07_Fleet_HyperdriveFailedNEW			(790 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	Hyperdrive jump failed!  The Quantum waveform collapsed due to some kind of inhibitor field.

#define M08_Fleet_CannotStay					(791 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_SINGLE_PLAYER)	//	We cannot stay - we're on a journey.  But let there be peace between us, for we have something in common.  The hyperdrive technology left to us by our ancestors is identical to yours.  The Homeworld we seek may be yours as well.

#define COMM_F_HyperspaceDetected				(792 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//  0.8 STATUS_HyperspaceDetected - 1) Hyperspace signature detected.
#define COMM_F_PrimaryObjectiveComplete			(793 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//  0.9 STATUS_PrimaryObjectiveComplete - 1) Primary objective complete.
#define COMM_F_SecondaryObjectiveComplete		(794 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_COMMAND)	//	0.10 STATUS_SecondaryObjectiveComplete - 1) Secondary objective complete.
#define STAT_F_CapturedShipRetrofit				(795 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_STATUS)	//	0.11 STATUS_CapturedShipBeingRetrofitted - 1) Captured ship secure.  Retrofit commencing.

/* PICKUPS - Final Animatic */
#define A16_Narrator_Ending						(796 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	The Galactic Council recognized our claim to this world.
#define A16_Narrator_Ending2					(797 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	The sacrifice of thousands has left a trail of destruction behind us, like a path across the galaxy
#define A16_Narrator_Ending3					(798 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	..to  Higaara, our Homeworld.
#define A16_Narrator_Ending4					(799 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	So much destruction, so many lives lost, for this place.
#define A16_Narrator_Ending5					(800 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	A place of wonder to those who knew only the sands of Kharak.
#define A16_Narrator_Ending6					(801 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	Our colonists were released from their long sleep.
#define A16_Narrator_Ending7					(802 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	All symbols of the old empire were destroyed.
#define A16_Narrator_Ending8					(803 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	But the conflict will never be forgotten.
#define A16_Narrator_Ending9					(804 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	A celebration marked the beginning of a new time.
#define A16_Narrator_Ending10					(805 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//	No longer Fleet Command, Karan Sjet survived extraction from the Mothership's core.
#define A16_Narrator_Ending11					(806 + SPEECH_ACTOR_NARRATOR + SPEECH_TYPE_ANIMATIC)	//  She insisted that she would be the last person to disembark and set foot on the Homeworld.

/* SCRAPPED EVENTS REQUESTED TO BE RE-ADDED */
#define SP_Pilot_SCVetteNotEnough				(807 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_SINGLE_PLAYER)	//	This is to tell the player they're not using enough SC Vettes.

/* damned Raider Retreat mission */
#define M04OEM_Intel_Hyperspace					(808 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We've located the origin of the Turanic Raider fleets.  They came from a small, isolated planetoid located in a remote area of the Great Wasteland.  Engage hyperspace.
#define M05OEM_Intel_PlanetaryDefense			(809 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Sensors indicate a heavily armed planetary defense system which we cannot penetrate.
#define M05OEM_Intel_HyperspacingIn				(810 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  The planetary garrison must have alerted the main Turanic Raider Fleet which is currently Hyperspacing in here.  They are retreating to the planetoid.
#define M05OEM_Intel_DestroyCarriers			(811 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  We must stop their Carriers from reaching the safety of the planetary defenses.
#define M05OEM_Intel_PrepareForAssault			(812 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	The Turanic Carriers are launching Strike Craft.  Prepare for assault.
#define M05OEM_Intel_MissionFailed				(813 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Mission Failed.
#define M05OEM_Intel_StayOutOfRange				(814 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//  The planetary defense system covers this area. Any of our ships entering its range will be destroyed.  Avoid this area at all costs.
#define M05OEM_Intel_DestroyedRaiders			(815 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	We have destroyed the body of the Turanic Raider fleet.  We can now proceed on our journey.  Hyperspace when ready.

/* Bug Fixes */
#define M04_Intel_DefendTheFleetShort			(816 + SPEECH_ACTOR_FLEET_INTELLIGENCE + SPEECH_TYPE_SINGLE_PLAYER)	//	Turanic Raider fighters are attacking. Prepare for more hostile ships to arrive.
#define STAT_Group_LowOnFuel					(817 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP + SPEECH_TYPE_ALWAYS_PLAY)	//	115. STATUS_StrikeCraft-LowOnFuel (ALL SHIPS)
#define STAT_Group_OutOfFuel					(818 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_STATUS + SPEECH_TYPE_GROUP + SPEECH_TYPE_ALWAYS_PLAY)	//	116. STATUS_StrikeCraft-OutOfFuel (ALL SHIPS)

/* alternate tutorial event for Falko */
#define TUT_SelectInfoOverlayAlt				(819 + SPEECH_ACTOR_FLEET_COMMAND + SPEECH_TYPE_TUTORIAL)	//	At the top right of the screen is the Info Overlay.  It displays all the selected ships.  Click Next to continue.

/* SCRAPPED EVENTS REQUESTED TO BE RE-ADDED */
#define SP_Pilot_P2Refuelling					(820 + SPEECH_ACTOR_PILOT + SPEECH_TYPE_SINGLE_PLAYER)	//	This tells the player to go after the fuel pods

/*********  FINALLY DONE!!!!  Maybe :)  ******/

#define SPEECH_LAST_EVENT						(SP_Pilot_P2Refuelling & SPEECH_EVENT_MASK)

#define NUM_SPEECH_EVENTS						(SPEECH_LAST_EVENT + 1)

#define SPEECH_FIRST_SP_EVENT					(TUT_Intro & SPEECH_EVENT_MASK)

#endif
