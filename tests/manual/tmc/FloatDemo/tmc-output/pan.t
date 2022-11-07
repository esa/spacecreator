#ifdef PEG
struct T_SRC {
	char *fl; int ln;
} T_SRC[NTRANS];

void
tr_2_src(int m, char *file, int ln)
{	T_SRC[m].fl = file;
	T_SRC[m].ln = ln;
}

void
putpeg(int n, int m)
{	printf("%5d	trans %4d ", m, n);
	printf("%s:%d\n",
		T_SRC[n].fl, T_SRC[n].ln);
}
#endif

void
settable(void)
{	Trans *T;
	Trans *settr(int, int, int, int, int, char *, int, int, int);

	trans = (Trans ***) emalloc(9*sizeof(Trans **));

	/* proctype 7: :init: */

	trans[7] = (Trans **) emalloc(204*sizeof(Trans *));

	T = trans[ 7][202] = settr(567,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(567,2,14,0,0,"ATOMIC", 1, 2500, 0);
	T = trans[ 7][14] = settr(379,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(379,0,13,0,0,"sub-sequence", 1, 2500, 0);
/*->*/	trans[7][13]	= settr(378,34,58,3,3,"D_STEP19091", 1, 2, 0);
	T = trans[ 7][58] = settr(423,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(423,0,18,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][18] = settr(383,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(383,0,15,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][15]	= settr(380,2,17,4,4,"global_state.sensor.sender = 4", 1, 2500, 0);
	T = trans[ 7][17] = settr(382,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(382,0,16,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][16]	= settr(381,2,57,5,0,"assert((((((global_state.sensor.sender==0)||(global_state.sensor.sender==1))||(global_state.sensor.sender==2))||(global_state.sensor.sender==3))||(global_state.sensor.sender==4)))", 1, 2500, 0);
	T = trans[ 7][57] = settr(422,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(422,0,19,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][19]	= settr(384,2,54,6,6,"transition_id = 0", 1, 2500, 0); /* m: 20 -> 0,54 */
	reached7[20] = 1;
	trans[7][20]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[7][55]	= settr(420,2,54,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][54] = settr(419,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(419,2,51,0,0,"DO", 1, 2500, 0);
	T = trans[7][51] = settr(416,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(416,2,21,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(416,2,23,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(416,2,35,0,0,"IF", 1, 2500, 0);
	trans[7][21]	= settr(386,2,59,7,7,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][22]	= settr(387,2,59,1,0,"goto :b12", 1, 2500, 0);
	trans[7][52]	= settr(417,2,53,1,0,".(goto)", 1, 2500, 0);
	trans[7][23]	= settr(388,2,27,8,8,"((transition_id==0))", 1, 2500, 0);
	T = trans[ 7][27] = settr(392,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(392,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][24]	= settr(389,2,26,9,9,"global_state.sensor.x = 1.000000", 1, 2500, 0);
	T = trans[ 7][26] = settr(391,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(391,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][25]	= settr(390,2,31,10,0,"assert(((global_state.sensor.x>=-(1000))&&(global_state.sensor.x<=1000)))", 1, 2500, 0);
	T = trans[ 7][31] = settr(396,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(396,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][28]	= settr(393,2,53,11,11,"global_state.timers.sensor.trigger.interval = 1000", 1, 2500, 0); /* m: 29 -> 0,53 */
	reached7[29] = 1;
	trans[7][29]	= settr(0,0,0,0,0,"global_state.timers.sensor.trigger.timer_enabled = 1",0,0,0);
	trans[7][30]	= settr(0,0,0,0,0,"printf('set_timer Sensor trigger %d\\n',1000)",0,0,0);
	trans[7][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][33]	= settr(0,0,0,0,0,"global_state.sensor.state = 0",0,0,0);
	trans[7][34]	= settr(399,2,53,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][35]	= settr(400,2,37,12,12,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][37] = settr(402,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(402,0,36,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][36]	= settr(401,2,41,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][41] = settr(406,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(406,0,38,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][38]	= settr(403,2,40,13,13,"global_state.sensor.x = (global_state.sensor.x*2.000000)", 1, 2500, 0);
	T = trans[ 7][40] = settr(405,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(405,0,39,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][39]	= settr(404,2,43,14,0,"assert(((global_state.sensor.x>=-(1000))&&(global_state.sensor.x<=1000)))", 1, 2500, 0);
	T = trans[ 7][43] = settr(408,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(408,0,42,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][42]	= settr(407,2,47,15,15,"Controlsystem_reading_channel!global_state.sensor.x", 1, 2500, 0);
	T = trans[ 7][47] = settr(412,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(412,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][44]	= settr(409,2,53,16,16,"global_state.timers.sensor.trigger.interval = 1000", 1, 2500, 0); /* m: 45 -> 0,53 */
	reached7[45] = 1;
	trans[7][45]	= settr(0,0,0,0,0,"global_state.timers.sensor.trigger.timer_enabled = 1",0,0,0);
	trans[7][46]	= settr(0,0,0,0,0,"printf('set_timer Sensor trigger %d\\n',1000)",0,0,0);
	trans[7][48]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][49]	= settr(0,0,0,0,0,"global_state.sensor.state = 0",0,0,0);
	trans[7][50]	= settr(415,2,53,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][53]	= settr(418,2,54,1,0,"(1)", 1, 2500, 0);
	trans[7][56]	= settr(421,2,59,1,0,"break", 1, 2500, 0);
	trans[7][59]	= settr(424,2,100,17,17,"Sensor_lock!1", 1, 2500, 0);
	T = trans[ 7][100] = settr(465,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(465,0,63,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][63] = settr(428,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(428,0,60,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][60]	= settr(425,2,62,18,18,"global_state.actuator.sender = 4", 1, 2500, 0);
	T = trans[ 7][62] = settr(427,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(427,0,61,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][61]	= settr(426,2,67,19,0,"assert((((((global_state.actuator.sender==0)||(global_state.actuator.sender==1))||(global_state.actuator.sender==2))||(global_state.actuator.sender==3))||(global_state.actuator.sender==4)))", 1, 2500, 0);
	T = trans[ 7][67] = settr(432,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(432,0,64,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][64]	= settr(429,2,66,20,20,"global_state.actuator.setting = 0.000000", 1, 2500, 0);
	T = trans[ 7][66] = settr(431,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(431,0,65,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][65]	= settr(430,2,99,21,0,"assert(((global_state.actuator.setting>=-(1000))&&(global_state.actuator.setting<=1000)))", 1, 2500, 0);
	T = trans[ 7][99] = settr(464,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(464,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][68]	= settr(433,2,96,22,22,"transition_id = 0", 1, 2500, 0); /* m: 69 -> 0,96 */
	reached7[69] = 1;
	trans[7][69]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[7][97]	= settr(462,2,96,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][96] = settr(461,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(461,2,93,0,0,"DO", 1, 2500, 0);
	T = trans[7][93] = settr(458,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(458,2,70,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(458,2,72,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(458,2,76,0,0,"IF", 1, 2500, 0);
	trans[7][70]	= settr(435,2,101,23,23,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][71]	= settr(436,2,101,1,0,"goto :b13", 1, 2500, 0);
	trans[7][94]	= settr(459,2,95,1,0,".(goto)", 1, 2500, 0);
	trans[7][72]	= settr(437,2,95,24,24,"((transition_id==0))", 1, 2500, 0); /* m: 73 -> 95,0 */
	reached7[73] = 1;
	trans[7][73]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][74]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[7][75]	= settr(440,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][76]	= settr(441,2,78,25,25,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][78] = settr(443,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(443,0,77,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][77]	= settr(442,2,82,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][82] = settr(447,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(447,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][79]	= settr(444,2,81,26,26,"global_state.actuator.setting = (1.500000*global_state.actuator.inputcontrol)", 1, 2500, 0);
	T = trans[ 7][81] = settr(446,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(446,0,80,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][80]	= settr(445,2,91,27,0,"assert(((global_state.actuator.setting>=-(1000))&&(global_state.actuator.setting<=1000)))", 1, 2500, 0);
	T = trans[7][91] = settr(456,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(456,2,83,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(456,2,87,0,0,"IF", 1, 2500, 0);
	trans[7][83]	= settr(448,2,95,28,28,"(((global_state.actuator.setting>10.000000)==1))", 1, 2500, 0); /* m: 84 -> 95,0 */
	reached7[84] = 1;
	trans[7][84]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][85]	= settr(0,0,0,0,0,"global_state.actuator.state = 1",0,0,0);
	trans[7][86]	= settr(451,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][92]	= settr(457,2,95,1,0,".(goto)", 1, 2500, 0);
	trans[7][87]	= settr(452,2,95,29,29,"(((global_state.actuator.setting>10.000000)==0))", 1, 2500, 0); /* m: 88 -> 95,0 */
	reached7[88] = 1;
	trans[7][88]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][89]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[7][90]	= settr(455,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][95]	= settr(460,2,96,1,0,"(1)", 1, 2500, 0);
	trans[7][98]	= settr(463,2,101,1,0,"break", 1, 2500, 0);
	trans[7][101]	= settr(466,2,163,30,30,"Actuator_lock!1", 1, 2500, 0);
	T = trans[ 7][163] = settr(528,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(528,0,105,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][105] = settr(470,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(470,0,102,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][102]	= settr(467,2,104,31,31,"global_state.controlsystem.sender = 4", 1, 2500, 0);
	T = trans[ 7][104] = settr(469,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(469,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][103]	= settr(468,2,109,32,0,"assert((((((global_state.controlsystem.sender==0)||(global_state.controlsystem.sender==1))||(global_state.controlsystem.sender==2))||(global_state.controlsystem.sender==3))||(global_state.controlsystem.sender==4)))", 1, 2500, 0);
	T = trans[ 7][109] = settr(474,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(474,0,106,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][106]	= settr(471,2,108,33,33,"global_state.controlsystem.i = 0.000000", 1, 2500, 0);
	T = trans[ 7][108] = settr(473,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(473,0,107,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][107]	= settr(472,2,113,34,0,"assert(((global_state.controlsystem.i>=-(1000))&&(global_state.controlsystem.i<=1000)))", 1, 2500, 0);
	T = trans[ 7][113] = settr(478,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(478,0,110,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][110]	= settr(475,2,112,35,35,"global_state.controlsystem.d = 0.000000", 1, 2500, 0);
	T = trans[ 7][112] = settr(477,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(477,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][111]	= settr(476,2,117,36,0,"assert(((global_state.controlsystem.d>=-(1000))&&(global_state.controlsystem.d<=1000)))", 1, 2500, 0);
	T = trans[ 7][117] = settr(482,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(482,0,114,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][114]	= settr(479,2,116,37,37,"global_state.controlsystem.previous = 0.000000", 1, 2500, 0);
	T = trans[ 7][116] = settr(481,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(481,0,115,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][115]	= settr(480,2,162,38,0,"assert(((global_state.controlsystem.previous>=-(1000))&&(global_state.controlsystem.previous<=1000)))", 1, 2500, 0);
	T = trans[ 7][162] = settr(527,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(527,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][118]	= settr(483,2,159,39,39,"transition_id = 0", 1, 2500, 0); /* m: 119 -> 0,159 */
	reached7[119] = 1;
	trans[7][119]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[7][160]	= settr(525,2,159,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][159] = settr(524,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(524,2,156,0,0,"DO", 1, 2500, 0);
	T = trans[7][156] = settr(521,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(521,2,120,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(521,2,122,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(521,2,126,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(521,2,132,0,0,"IF", 1, 2500, 0);
	trans[7][120]	= settr(485,2,164,40,40,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][121]	= settr(486,2,164,1,0,"goto :b14", 1, 2500, 0);
	trans[7][157]	= settr(522,2,158,1,0,".(goto)", 1, 2500, 0);
	trans[7][122]	= settr(487,2,158,41,41,"((transition_id==0))", 1, 2500, 0); /* m: 123 -> 158,0 */
	reached7[123] = 1;
	trans[7][123]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][124]	= settr(0,0,0,0,0,"global_state.controlsystem.state = 1",0,0,0);
	trans[7][125]	= settr(490,2,158,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][126]	= settr(491,2,128,42,42,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][128] = settr(493,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(493,0,127,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][127]	= settr(492,2,158,43,43,"(1)", 1, 2500, 0); /* m: 129 -> 158,0 */
	reached7[129] = 1;
	trans[7][129]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][130]	= settr(0,0,0,0,0,"global_state.controlsystem.state = 0",0,0,0);
	trans[7][131]	= settr(496,2,158,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][132]	= settr(497,2,134,44,44,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 7][134] = settr(499,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(499,0,133,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][133]	= settr(498,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][147] = settr(512,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(512,0,138,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][138] = settr(503,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(503,0,135,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][135]	= settr(500,2,137,45,45,"global_state.controlsystem.i = (global_state.controlsystem.i+global_state.controlsystem.inputvalue)", 1, 2500, 0);
	T = trans[ 7][137] = settr(502,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(502,0,136,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][136]	= settr(501,2,142,46,0,"assert(((global_state.controlsystem.i>=-(1000))&&(global_state.controlsystem.i<=1000)))", 1, 2500, 0);
	T = trans[ 7][142] = settr(507,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(507,0,139,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][139]	= settr(504,2,141,47,47,"global_state.controlsystem.d = (global_state.controlsystem.inputvalue-global_state.controlsystem.previous)", 1, 2500, 0);
	T = trans[ 7][141] = settr(506,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(506,0,140,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][140]	= settr(505,2,146,48,0,"assert(((global_state.controlsystem.d>=-(1000))&&(global_state.controlsystem.d<=1000)))", 1, 2500, 0);
	T = trans[ 7][146] = settr(511,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(511,0,143,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][143]	= settr(508,2,145,49,49,"global_state.controlsystem.previous = global_state.controlsystem.inputvalue", 1, 2500, 0);
	T = trans[ 7][145] = settr(510,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(510,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][144]	= settr(509,2,151,50,0,"assert(((global_state.controlsystem.previous>=-(1000))&&(global_state.controlsystem.previous<=1000)))", 1, 2500, 0);
	T = trans[ 7][151] = settr(516,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(516,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][148]	= settr(513,2,150,51,51,"global_state.controlsystem.outputvalue = (((global_state.controlsystem.cfg.kp*global_state.controlsystem.inputvalue)+(global_state.controlsystem.cfg.ki*global_state.controlsystem.i))+(global_state.controlsystem.cfg.kd*global_state.controlsystem.d))", 1, 2500, 0);
	T = trans[ 7][150] = settr(515,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(515,0,149,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][149]	= settr(514,2,153,52,0,"assert(((global_state.controlsystem.outputvalue>=-(1000))&&(global_state.controlsystem.outputvalue<=1000)))", 1, 2500, 0);
	T = trans[ 7][153] = settr(518,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(518,0,152,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][152]	= settr(517,2,158,53,53,"Actuator_controlSignal_channel!global_state.controlsystem.outputvalue", 1, 2500, 0); /* m: 154 -> 158,0 */
	reached7[154] = 1;
	trans[7][154]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][155]	= settr(520,2,158,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][158]	= settr(523,2,159,1,0,"(1)", 1, 2500, 0);
	trans[7][161]	= settr(526,2,164,1,0,"break", 1, 2500, 0);
	trans[7][164]	= settr(529,2,199,54,54,"Controlsystem_lock!1", 1, 2500, 0);
	T = trans[ 7][199] = settr(564,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(564,0,168,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][168] = settr(533,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(533,0,165,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][165]	= settr(530,2,167,55,55,"global_state.host.sender = 4", 1, 2500, 0);
	T = trans[ 7][167] = settr(532,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(532,0,166,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][166]	= settr(531,2,198,56,0,"assert((((((global_state.host.sender==0)||(global_state.host.sender==1))||(global_state.host.sender==2))||(global_state.host.sender==3))||(global_state.host.sender==4)))", 1, 2500, 0);
	T = trans[ 7][198] = settr(563,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(563,0,169,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][169]	= settr(534,2,195,57,57,"transition_id = 0", 1, 2500, 0); /* m: 170 -> 0,195 */
	reached7[170] = 1;
	trans[7][170]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[7][196]	= settr(561,2,195,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][195] = settr(560,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(560,2,192,0,0,"DO", 1, 2500, 0);
	T = trans[7][192] = settr(557,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(557,2,171,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(557,2,173,0,0,"IF", 1, 2500, 0);
	trans[7][171]	= settr(536,2,200,58,58,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][172]	= settr(537,2,200,1,0,"goto :b15", 1, 2500, 0);
	trans[7][193]	= settr(558,2,194,1,0,".(goto)", 1, 2500, 0);
	trans[7][173]	= settr(538,2,186,59,59,"((transition_id==0))", 1, 2500, 0);
	T = trans[ 7][186] = settr(551,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(551,0,177,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][177] = settr(542,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(542,0,174,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][174]	= settr(539,2,176,60,60,"global_state.host.cfg.kp = 1.300000", 1, 2500, 0);
	T = trans[ 7][176] = settr(541,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(541,0,175,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][175]	= settr(540,2,181,61,0,"assert(((global_state.host.cfg.kp>=-(1000))&&(global_state.host.cfg.kp<=1000)))", 1, 2500, 0);
	T = trans[ 7][181] = settr(546,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(546,0,178,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][178]	= settr(543,2,180,62,62,"global_state.host.cfg.kd = 2.100000", 1, 2500, 0);
	T = trans[ 7][180] = settr(545,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(545,0,179,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][179]	= settr(544,2,185,63,0,"assert(((global_state.host.cfg.kd>=-(1000))&&(global_state.host.cfg.kd<=1000)))", 1, 2500, 0);
	T = trans[ 7][185] = settr(550,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(550,0,182,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][182]	= settr(547,2,184,64,64,"global_state.host.cfg.ki = 3.200000", 1, 2500, 0);
	T = trans[ 7][184] = settr(549,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(549,0,183,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][183]	= settr(548,2,188,65,0,"assert(((global_state.host.cfg.ki>=-(1000))&&(global_state.host.cfg.ki<=1000)))", 1, 2500, 0);
	T = trans[ 7][188] = settr(553,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(553,0,187,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][187]	= settr(552,2,194,66,66,"Controlsystem_configuration_channel!global_state.host.cfg.kp,global_state.host.cfg.ki,global_state.host.cfg.kd", 1, 2500, 0); /* m: 189 -> 194,0 */
	reached7[189] = 1;
	trans[7][189]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][190]	= settr(0,0,0,0,0,"global_state.host.state = 0",0,0,0);
	trans[7][191]	= settr(556,2,194,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][194]	= settr(559,2,195,1,0,"(1)", 1, 2500, 0);
	trans[7][197]	= settr(562,2,200,1,0,"break", 1, 2500, 0);
	trans[7][200]	= settr(565,4,203,67,67,"Host_lock!1", 1, 2500, 0); /* m: 201 -> 203,0 */
	reached7[201] = 1;
	trans[7][201]	= settr(0,0,0,0,0,"inited = 1",0,0,0);
	trans[7][203]	= settr(568,0,0,68,68,"-end-", 0, 3500, 0);

	/* proctype 6: Sensor_trigger */

	trans[6] = (Trans **) emalloc(62*sizeof(Trans *));

	trans[6][1]	= settr(305,0,58,69,0,"(inited)", 1, 2, 0);
	trans[6][59]	= settr(363,0,58,1,0,".(goto)", 0, 2, 0);
	T = trans[6][58] = settr(362,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(362,0,57,0,0,"DO", 0, 2, 0);
	T = trans[ 6][57] = settr(361,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(361,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[6][2]	= settr(306,2,3,70,0,"(nempty(Sensor_trigger_channel))", 1, 2500, 0);
	trans[6][3]	= settr(307,2,54,71,71,"Sensor_lock?_", 1, 2500, 0);
	T = trans[6][54] = settr(358,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(358,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(358,2,52,0,0,"IF", 1, 2500, 0);
	trans[6][4]	= settr(308,2,5,72,0,"(nempty(Sensor_trigger_channel))", 1, 2500, 0);
	trans[6][5]	= settr(309,2,50,73,73,"Sensor_trigger_channel?_", 1, 2500, 0);
	T = trans[ 6][50] = settr(354,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(354,0,48,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[6][48] = settr(352,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(352,2,6,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(352,2,46,0,0,"IF", 1, 2500, 0);
	trans[6][6]	= settr(310,2,45,74,0,"((global_state.sensor.state==0))", 1, 2500, 0);
	T = trans[ 6][45] = settr(349,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(349,0,7,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][7]	= settr(311,2,42,75,75,"transition_id = 0", 1, 2500, 0); /* m: 8 -> 0,42 */
	reached6[8] = 1;
	trans[6][8]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[6][43]	= settr(347,2,42,1,0,".(goto)", 1, 2500, 0);
	T = trans[6][42] = settr(346,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(346,2,39,0,0,"DO", 1, 2500, 0);
	T = trans[6][39] = settr(343,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(343,2,9,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(343,2,11,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(343,2,23,0,0,"IF", 1, 2500, 0);
	trans[6][9]	= settr(313,2,54,76,76,"((transition_id==-(1)))", 1, 2500, 0);
	trans[6][10]	= settr(314,2,54,1,0,"goto :b11", 1, 2500, 0);
	trans[6][40]	= settr(344,2,41,1,0,".(goto)", 1, 2500, 0);
	trans[6][11]	= settr(315,2,15,77,77,"((transition_id==0))", 1, 2500, 0);
	T = trans[ 6][15] = settr(319,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(319,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][12]	= settr(316,2,14,78,78,"global_state.sensor.x = 1.000000", 1, 2500, 0);
	T = trans[ 6][14] = settr(318,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(318,0,13,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][13]	= settr(317,2,19,79,0,"assert(((global_state.sensor.x>=-(1000))&&(global_state.sensor.x<=1000)))", 1, 2500, 0);
	T = trans[ 6][19] = settr(323,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(323,0,16,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][16]	= settr(320,2,41,80,80,"global_state.timers.sensor.trigger.interval = 1000", 1, 2500, 0); /* m: 17 -> 0,41 */
	reached6[17] = 1;
	trans[6][17]	= settr(0,0,0,0,0,"global_state.timers.sensor.trigger.timer_enabled = 1",0,0,0);
	trans[6][18]	= settr(0,0,0,0,0,"printf('set_timer Sensor trigger %d\\n',1000)",0,0,0);
	trans[6][20]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][21]	= settr(0,0,0,0,0,"global_state.sensor.state = 0",0,0,0);
	trans[6][22]	= settr(326,2,41,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][23]	= settr(327,2,25,81,81,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 6][25] = settr(329,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(329,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][24]	= settr(328,2,29,1,0,"(1)", 1, 2500, 0);
	T = trans[ 6][29] = settr(333,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(333,0,26,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][26]	= settr(330,2,28,82,82,"global_state.sensor.x = (global_state.sensor.x*2.000000)", 1, 2500, 0);
	T = trans[ 6][28] = settr(332,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(332,0,27,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][27]	= settr(331,2,31,83,0,"assert(((global_state.sensor.x>=-(1000))&&(global_state.sensor.x<=1000)))", 1, 2500, 0);
	T = trans[ 6][31] = settr(335,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(335,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][30]	= settr(334,2,35,84,84,"Controlsystem_reading_channel!global_state.sensor.x", 1, 2500, 0);
	T = trans[ 6][35] = settr(339,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(339,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][32]	= settr(336,2,41,85,85,"global_state.timers.sensor.trigger.interval = 1000", 1, 2500, 0); /* m: 33 -> 0,41 */
	reached6[33] = 1;
	trans[6][33]	= settr(0,0,0,0,0,"global_state.timers.sensor.trigger.timer_enabled = 1",0,0,0);
	trans[6][34]	= settr(0,0,0,0,0,"printf('set_timer Sensor trigger %d\\n',1000)",0,0,0);
	trans[6][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][37]	= settr(0,0,0,0,0,"global_state.sensor.state = 0",0,0,0);
	trans[6][38]	= settr(342,2,41,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][41]	= settr(345,2,42,1,0,"(1)", 1, 2500, 0);
	trans[6][44]	= settr(348,2,54,1,0,"break", 1, 2500, 0);
	trans[6][49]	= settr(353,2,54,1,0,".(goto)", 1, 2500, 0);
	trans[6][46]	= settr(350,2,47,2,0,"else", 1, 2500, 0);
	trans[6][47]	= settr(351,2,54,1,0,"(1)", 1, 2500, 0);
	trans[6][51]	= settr(355,2,54,1,0,"goto Sensor_trigger_loop", 1, 2500, 0);
	trans[6][55]	= settr(359,2,56,1,0,".(goto)", 1, 2500, 0);
	trans[6][52]	= settr(356,2,53,86,0,"(empty(Sensor_trigger_channel))", 1, 2500, 0);
	trans[6][53]	= settr(357,2,56,87,87,"(1)", 1, 2500, 0); /* m: 55 -> 56,0 */
	reached6[55] = 1;
	trans[6][56]	= settr(360,0,58,88,88,"Sensor_lock!1", 1, 2500, 0);
	trans[6][60]	= settr(364,0,61,1,0,"break", 0, 2, 0);
	trans[6][61]	= settr(365,0,0,89,89,"-end-", 0, 3500, 0);

	/* proctype 5: Sensor_tc */

	trans[5] = (Trans **) emalloc(22*sizeof(Trans *));

	trans[5][1]	= settr(284,0,18,90,0,"(inited)", 1, 2, 0);
	trans[5][19]	= settr(302,0,18,1,0,".(goto)", 0, 2, 0);
	T = trans[5][18] = settr(301,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(301,0,17,0,0,"DO", 0, 2, 0);
	T = trans[ 5][17] = settr(300,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(300,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[5][2]	= settr(285,2,3,91,0,"(nempty(Sensor_tc_channel))", 1, 2500, 0);
	trans[5][3]	= settr(286,2,14,92,92,"Sensor_lock?_", 1, 2500, 0);
	T = trans[5][14] = settr(297,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(297,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(297,2,12,0,0,"IF", 1, 2500, 0);
	trans[5][4]	= settr(287,2,5,93,0,"(nempty(Sensor_tc_channel))", 1, 2500, 0);
	trans[5][5]	= settr(288,2,10,94,94,"Sensor_tc_channel?_", 1, 2500, 0);
	T = trans[ 5][10] = settr(293,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(293,0,8,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[5][8] = settr(291,2,0,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(291,2,6,0,0,"IF", 1, 2500, 0);
	trans[5][6]	= settr(289,2,7,2,0,"else", 1, 2500, 0);
	trans[5][7]	= settr(290,2,14,1,0,"(1)", 1, 2500, 0);
	trans[5][9]	= settr(292,2,14,1,0,".(goto)", 1, 2500, 0);
	trans[5][11]	= settr(294,2,14,1,0,"goto Sensor_tc_loop", 1, 2500, 0);
	trans[5][15]	= settr(298,2,16,1,0,".(goto)", 1, 2500, 0);
	trans[5][12]	= settr(295,2,13,95,0,"(empty(Sensor_tc_channel))", 1, 2500, 0);
	trans[5][13]	= settr(296,2,16,96,96,"(1)", 1, 2500, 0); /* m: 15 -> 16,0 */
	reached5[15] = 1;
	trans[5][16]	= settr(299,0,18,97,97,"Sensor_lock!1", 1, 2500, 0);
	trans[5][20]	= settr(303,0,21,1,0,"break", 0, 2, 0);
	trans[5][21]	= settr(304,0,0,98,98,"-end-", 0, 3500, 0);

	/* proctype 4: Host_tm */

	trans[4] = (Trans **) emalloc(22*sizeof(Trans *));

	trans[4][1]	= settr(263,0,18,99,0,"(inited)", 1, 2, 0);
	trans[4][19]	= settr(281,0,18,1,0,".(goto)", 0, 2, 0);
	T = trans[4][18] = settr(280,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(280,0,17,0,0,"DO", 0, 2, 0);
	T = trans[ 4][17] = settr(279,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(279,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[4][2]	= settr(264,2,3,100,0,"(nempty(Host_tm_channel))", 1, 2500, 0);
	trans[4][3]	= settr(265,2,14,101,101,"Host_lock?_", 1, 2500, 0);
	T = trans[4][14] = settr(276,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(276,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(276,2,12,0,0,"IF", 1, 2500, 0);
	trans[4][4]	= settr(266,2,5,102,0,"(nempty(Host_tm_channel))", 1, 2500, 0);
	trans[4][5]	= settr(267,2,10,103,103,"Host_tm_channel?_", 1, 2500, 0);
	T = trans[ 4][10] = settr(272,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(272,0,8,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[4][8] = settr(270,2,0,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(270,2,6,0,0,"IF", 1, 2500, 0);
	trans[4][6]	= settr(268,2,7,2,0,"else", 1, 2500, 0);
	trans[4][7]	= settr(269,2,14,1,0,"(1)", 1, 2500, 0);
	trans[4][9]	= settr(271,2,14,1,0,".(goto)", 1, 2500, 0);
	trans[4][11]	= settr(273,2,14,1,0,"goto Host_tm_loop", 1, 2500, 0);
	trans[4][15]	= settr(277,2,16,1,0,".(goto)", 1, 2500, 0);
	trans[4][12]	= settr(274,2,13,104,0,"(empty(Host_tm_channel))", 1, 2500, 0);
	trans[4][13]	= settr(275,2,16,105,105,"(1)", 1, 2500, 0); /* m: 15 -> 16,0 */
	reached4[15] = 1;
	trans[4][16]	= settr(278,0,18,106,106,"Host_lock!1", 1, 2500, 0);
	trans[4][20]	= settr(282,0,21,1,0,"break", 0, 2, 0);
	trans[4][21]	= settr(283,0,0,107,107,"-end-", 0, 3500, 0);

	/* proctype 3: Controlsystem_reading */

	trans[3] = (Trans **) emalloc(73*sizeof(Trans *));

	trans[3][1]	= settr(191,0,69,108,0,"(inited)", 1, 2, 0);
	trans[3][70]	= settr(260,0,69,1,0,".(goto)", 0, 2, 0);
	T = trans[3][69] = settr(259,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(259,0,68,0,0,"DO", 0, 2, 0);
	T = trans[ 3][68] = settr(258,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(258,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[3][2]	= settr(192,2,3,109,0,"(nempty(Controlsystem_reading_channel))", 1, 2500, 0);
	trans[3][3]	= settr(193,2,65,110,110,"Controlsystem_lock?_", 1, 2500, 0);
	T = trans[3][65] = settr(255,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(255,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(255,2,63,0,0,"IF", 1, 2500, 0);
	trans[3][4]	= settr(194,2,5,111,0,"(nempty(Controlsystem_reading_channel))", 1, 2500, 0);
	trans[3][5]	= settr(195,2,61,112,112,"Controlsystem_reading_channel?Controlsystem_reading_signal_parameter", 1, 2500, 0); /* m: 6 -> 61,0 */
	reached3[6] = 1;
	trans[3][6]	= settr(0,0,0,0,0,"Controlsystem_reading_channel_used = 1",0,0,0);
	T = trans[ 3][61] = settr(251,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(251,0,59,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[3][59] = settr(249,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(249,2,7,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(249,2,57,0,0,"IF", 1, 2500, 0);
	trans[3][7]	= settr(197,2,11,113,0,"((global_state.controlsystem.state==0))", 1, 2500, 0);
	T = trans[ 3][11] = settr(201,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(201,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][8]	= settr(198,2,10,114,114,"global_state.controlsystem.inputvalue = Controlsystem_reading_signal_parameter", 1, 2500, 0);
	T = trans[ 3][10] = settr(200,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(200,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][9]	= settr(199,2,56,115,0,"assert(((global_state.controlsystem.inputvalue>=-(1000))&&(global_state.controlsystem.inputvalue<=1000)))", 1, 2500, 0);
	T = trans[ 3][56] = settr(246,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(246,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][12]	= settr(202,2,53,116,116,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,53 */
	reached3[13] = 1;
	trans[3][13]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[3][54]	= settr(244,2,53,1,0,".(goto)", 1, 2500, 0);
	T = trans[3][53] = settr(243,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(243,2,50,0,0,"DO", 1, 2500, 0);
	T = trans[3][50] = settr(240,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(240,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(240,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(240,2,20,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(240,2,26,0,0,"IF", 1, 2500, 0);
	trans[3][14]	= settr(204,2,65,117,117,"((transition_id==-(1)))", 1, 2500, 0);
	trans[3][15]	= settr(205,2,65,1,0,"goto :b7", 1, 2500, 0);
	trans[3][51]	= settr(241,2,52,1,0,".(goto)", 1, 2500, 0);
	trans[3][16]	= settr(206,2,52,118,118,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 52,0 */
	reached3[17] = 1;
	trans[3][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[3][18]	= settr(0,0,0,0,0,"global_state.controlsystem.state = 1",0,0,0);
	trans[3][19]	= settr(209,2,52,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[3][20]	= settr(210,2,22,119,119,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 3][22] = settr(212,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(212,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][21]	= settr(211,2,52,120,120,"(1)", 1, 2500, 0); /* m: 23 -> 52,0 */
	reached3[23] = 1;
	trans[3][23]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[3][24]	= settr(0,0,0,0,0,"global_state.controlsystem.state = 0",0,0,0);
	trans[3][25]	= settr(215,2,52,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[3][26]	= settr(216,2,28,121,121,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 3][28] = settr(218,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(218,0,27,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][27]	= settr(217,2,41,1,0,"(1)", 1, 2500, 0);
	T = trans[ 3][41] = settr(231,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(231,0,32,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 3][32] = settr(222,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(222,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][29]	= settr(219,2,31,122,122,"global_state.controlsystem.i = (global_state.controlsystem.i+global_state.controlsystem.inputvalue)", 1, 2500, 0);
	T = trans[ 3][31] = settr(221,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(221,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][30]	= settr(220,2,36,123,0,"assert(((global_state.controlsystem.i>=-(1000))&&(global_state.controlsystem.i<=1000)))", 1, 2500, 0);
	T = trans[ 3][36] = settr(226,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(226,0,33,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][33]	= settr(223,2,35,124,124,"global_state.controlsystem.d = (global_state.controlsystem.inputvalue-global_state.controlsystem.previous)", 1, 2500, 0);
	T = trans[ 3][35] = settr(225,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(225,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][34]	= settr(224,2,40,125,0,"assert(((global_state.controlsystem.d>=-(1000))&&(global_state.controlsystem.d<=1000)))", 1, 2500, 0);
	T = trans[ 3][40] = settr(230,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(230,0,37,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][37]	= settr(227,2,39,126,126,"global_state.controlsystem.previous = global_state.controlsystem.inputvalue", 1, 2500, 0);
	T = trans[ 3][39] = settr(229,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(229,0,38,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][38]	= settr(228,2,45,127,0,"assert(((global_state.controlsystem.previous>=-(1000))&&(global_state.controlsystem.previous<=1000)))", 1, 2500, 0);
	T = trans[ 3][45] = settr(235,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(235,0,42,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][42]	= settr(232,2,44,128,128,"global_state.controlsystem.outputvalue = (((global_state.controlsystem.cfg.kp*global_state.controlsystem.inputvalue)+(global_state.controlsystem.cfg.ki*global_state.controlsystem.i))+(global_state.controlsystem.cfg.kd*global_state.controlsystem.d))", 1, 2500, 0);
	T = trans[ 3][44] = settr(234,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(234,0,43,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][43]	= settr(233,2,47,129,0,"assert(((global_state.controlsystem.outputvalue>=-(1000))&&(global_state.controlsystem.outputvalue<=1000)))", 1, 2500, 0);
	T = trans[ 3][47] = settr(237,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(237,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][46]	= settr(236,2,52,130,130,"Actuator_controlSignal_channel!global_state.controlsystem.outputvalue", 1, 2500, 0); /* m: 48 -> 52,0 */
	reached3[48] = 1;
	trans[3][48]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[3][49]	= settr(239,2,52,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[3][52]	= settr(242,2,53,1,0,"(1)", 1, 2500, 0);
	trans[3][55]	= settr(245,2,65,1,0,"break", 1, 2500, 0);
	trans[3][60]	= settr(250,2,65,1,0,".(goto)", 1, 2500, 0);
	trans[3][57]	= settr(247,2,58,2,0,"else", 1, 2500, 0);
	trans[3][58]	= settr(248,2,65,1,0,"(1)", 1, 2500, 0);
	trans[3][62]	= settr(252,2,65,1,0,"goto Controlsystem_reading_loop", 1, 2500, 0);
	trans[3][66]	= settr(256,2,67,1,0,".(goto)", 1, 2500, 0);
	trans[3][63]	= settr(253,2,64,131,0,"(empty(Controlsystem_reading_channel))", 1, 2500, 0);
	trans[3][64]	= settr(254,2,67,132,132,"(1)", 1, 2500, 0); /* m: 66 -> 67,0 */
	reached3[66] = 1;
	trans[3][67]	= settr(257,0,69,133,133,"Controlsystem_lock!1", 1, 2500, 0);
	trans[3][71]	= settr(261,0,72,1,0,"break", 0, 2, 0);
	trans[3][72]	= settr(262,0,0,134,134,"-end-", 0, 3500, 0);

	/* proctype 2: Controlsystem_configuration */

	trans[2] = (Trans **) emalloc(82*sizeof(Trans *));

	trans[2][1]	= settr(110,0,78,135,0,"(inited)", 1, 2, 0);
	trans[2][79]	= settr(188,0,78,1,0,".(goto)", 0, 2, 0);
	T = trans[2][78] = settr(187,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(187,0,77,0,0,"DO", 0, 2, 0);
	T = trans[ 2][77] = settr(186,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(186,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[2][2]	= settr(111,2,3,136,0,"(nempty(Controlsystem_configuration_channel))", 1, 2500, 0);
	trans[2][3]	= settr(112,2,74,137,137,"Controlsystem_lock?_", 1, 2500, 0);
	T = trans[2][74] = settr(183,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(183,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(183,2,72,0,0,"IF", 1, 2500, 0);
	trans[2][4]	= settr(113,2,5,138,0,"(nempty(Controlsystem_configuration_channel))", 1, 2500, 0);
	trans[2][5]	= settr(114,2,70,139,139,"Controlsystem_configuration_channel?Controlsystem_configuration_signal_parameter.kp,Controlsystem_configuration_signal_parameter.ki,Controlsystem_configuration_signal_parameter.kd", 1, 2500, 0); /* m: 6 -> 70,0 */
	reached2[6] = 1;
	trans[2][6]	= settr(0,0,0,0,0,"Controlsystem_configuration_channel_used = 1",0,0,0);
	T = trans[ 2][70] = settr(179,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(179,0,68,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[2][68] = settr(177,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(177,2,7,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(177,2,66,0,0,"IF", 1, 2500, 0);
	trans[2][7]	= settr(116,2,20,140,0,"((global_state.controlsystem.state==1))", 1, 2500, 0);
	T = trans[ 2][20] = settr(129,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(129,0,11,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 2][11] = settr(120,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(120,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][8]	= settr(117,2,10,141,141,"global_state.controlsystem.cfg.kp = Controlsystem_configuration_signal_parameter.kp", 1, 2500, 0);
	T = trans[ 2][10] = settr(119,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(119,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][9]	= settr(118,2,15,142,0,"assert(((global_state.controlsystem.cfg.kp>=-(1000))&&(global_state.controlsystem.cfg.kp<=1000)))", 1, 2500, 0);
	T = trans[ 2][15] = settr(124,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(124,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][12]	= settr(121,2,14,143,143,"global_state.controlsystem.cfg.ki = Controlsystem_configuration_signal_parameter.ki", 1, 2500, 0);
	T = trans[ 2][14] = settr(123,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(123,0,13,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][13]	= settr(122,2,19,144,0,"assert(((global_state.controlsystem.cfg.ki>=-(1000))&&(global_state.controlsystem.cfg.ki<=1000)))", 1, 2500, 0);
	T = trans[ 2][19] = settr(128,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(128,0,16,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][16]	= settr(125,2,18,145,145,"global_state.controlsystem.cfg.kd = Controlsystem_configuration_signal_parameter.kd", 1, 2500, 0);
	T = trans[ 2][18] = settr(127,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(127,0,17,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][17]	= settr(126,2,65,146,0,"assert(((global_state.controlsystem.cfg.kd>=-(1000))&&(global_state.controlsystem.cfg.kd<=1000)))", 1, 2500, 0);
	T = trans[ 2][65] = settr(174,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(174,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][21]	= settr(130,2,62,147,147,"transition_id = 0", 1, 2500, 0); /* m: 22 -> 0,62 */
	reached2[22] = 1;
	trans[2][22]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[2][63]	= settr(172,2,62,1,0,".(goto)", 1, 2500, 0);
	T = trans[2][62] = settr(171,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(171,2,59,0,0,"DO", 1, 2500, 0);
	T = trans[2][59] = settr(168,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(168,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(168,2,25,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(168,2,29,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(168,2,35,0,0,"IF", 1, 2500, 0);
	trans[2][23]	= settr(132,2,74,148,148,"((transition_id==-(1)))", 1, 2500, 0);
	trans[2][24]	= settr(133,2,74,1,0,"goto :b5", 1, 2500, 0);
	trans[2][60]	= settr(169,2,61,1,0,".(goto)", 1, 2500, 0);
	trans[2][25]	= settr(134,2,61,149,149,"((transition_id==0))", 1, 2500, 0); /* m: 26 -> 61,0 */
	reached2[26] = 1;
	trans[2][26]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][27]	= settr(0,0,0,0,0,"global_state.controlsystem.state = 1",0,0,0);
	trans[2][28]	= settr(137,2,61,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][29]	= settr(138,2,31,150,150,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 2][31] = settr(140,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(140,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][30]	= settr(139,2,61,151,151,"(1)", 1, 2500, 0); /* m: 32 -> 61,0 */
	reached2[32] = 1;
	trans[2][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][33]	= settr(0,0,0,0,0,"global_state.controlsystem.state = 0",0,0,0);
	trans[2][34]	= settr(143,2,61,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][35]	= settr(144,2,37,152,152,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 2][37] = settr(146,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(146,0,36,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][36]	= settr(145,2,50,1,0,"(1)", 1, 2500, 0);
	T = trans[ 2][50] = settr(159,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(159,0,41,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 2][41] = settr(150,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(150,0,38,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][38]	= settr(147,2,40,153,153,"global_state.controlsystem.i = (global_state.controlsystem.i+global_state.controlsystem.inputvalue)", 1, 2500, 0);
	T = trans[ 2][40] = settr(149,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(149,0,39,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][39]	= settr(148,2,45,154,0,"assert(((global_state.controlsystem.i>=-(1000))&&(global_state.controlsystem.i<=1000)))", 1, 2500, 0);
	T = trans[ 2][45] = settr(154,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(154,0,42,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][42]	= settr(151,2,44,155,155,"global_state.controlsystem.d = (global_state.controlsystem.inputvalue-global_state.controlsystem.previous)", 1, 2500, 0);
	T = trans[ 2][44] = settr(153,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(153,0,43,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][43]	= settr(152,2,49,156,0,"assert(((global_state.controlsystem.d>=-(1000))&&(global_state.controlsystem.d<=1000)))", 1, 2500, 0);
	T = trans[ 2][49] = settr(158,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(158,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][46]	= settr(155,2,48,157,157,"global_state.controlsystem.previous = global_state.controlsystem.inputvalue", 1, 2500, 0);
	T = trans[ 2][48] = settr(157,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(157,0,47,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][47]	= settr(156,2,54,158,0,"assert(((global_state.controlsystem.previous>=-(1000))&&(global_state.controlsystem.previous<=1000)))", 1, 2500, 0);
	T = trans[ 2][54] = settr(163,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(163,0,51,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][51]	= settr(160,2,53,159,159,"global_state.controlsystem.outputvalue = (((global_state.controlsystem.cfg.kp*global_state.controlsystem.inputvalue)+(global_state.controlsystem.cfg.ki*global_state.controlsystem.i))+(global_state.controlsystem.cfg.kd*global_state.controlsystem.d))", 1, 2500, 0);
	T = trans[ 2][53] = settr(162,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(162,0,52,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][52]	= settr(161,2,56,160,0,"assert(((global_state.controlsystem.outputvalue>=-(1000))&&(global_state.controlsystem.outputvalue<=1000)))", 1, 2500, 0);
	T = trans[ 2][56] = settr(165,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(165,0,55,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][55]	= settr(164,2,61,161,161,"Actuator_controlSignal_channel!global_state.controlsystem.outputvalue", 1, 2500, 0); /* m: 57 -> 61,0 */
	reached2[57] = 1;
	trans[2][57]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][58]	= settr(167,2,61,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][61]	= settr(170,2,62,1,0,"(1)", 1, 2500, 0);
	trans[2][64]	= settr(173,2,74,1,0,"break", 1, 2500, 0);
	trans[2][69]	= settr(178,2,74,1,0,".(goto)", 1, 2500, 0);
	trans[2][66]	= settr(175,2,67,2,0,"else", 1, 2500, 0);
	trans[2][67]	= settr(176,2,74,1,0,"(1)", 1, 2500, 0);
	trans[2][71]	= settr(180,2,74,1,0,"goto Controlsystem_configuration_loop", 1, 2500, 0);
	trans[2][75]	= settr(184,2,76,1,0,".(goto)", 1, 2500, 0);
	trans[2][72]	= settr(181,2,73,162,0,"(empty(Controlsystem_configuration_channel))", 1, 2500, 0);
	trans[2][73]	= settr(182,2,76,163,163,"(1)", 1, 2500, 0); /* m: 75 -> 76,0 */
	reached2[75] = 1;
	trans[2][76]	= settr(185,0,78,164,164,"Controlsystem_lock!1", 1, 2500, 0);
	trans[2][80]	= settr(189,0,81,1,0,"break", 0, 2, 0);
	trans[2][81]	= settr(190,0,0,165,165,"-end-", 0, 3500, 0);

	/* proctype 1: Actuator_controlSignal */

	trans[1] = (Trans **) emalloc(97*sizeof(Trans *));

	trans[1][1]	= settr(14,0,93,166,0,"(inited)", 1, 2, 0);
	trans[1][94]	= settr(107,0,93,1,0,".(goto)", 0, 2, 0);
	T = trans[1][93] = settr(106,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(106,0,92,0,0,"DO", 0, 2, 0);
	T = trans[ 1][92] = settr(105,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(105,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[1][2]	= settr(15,2,3,167,0,"(nempty(Actuator_controlSignal_channel))", 1, 2500, 0);
	trans[1][3]	= settr(16,2,89,168,168,"Actuator_lock?_", 1, 2500, 0);
	T = trans[1][89] = settr(102,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(102,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(102,2,87,0,0,"IF", 1, 2500, 0);
	trans[1][4]	= settr(17,2,5,169,0,"(nempty(Actuator_controlSignal_channel))", 1, 2500, 0);
	trans[1][5]	= settr(18,2,85,170,170,"Actuator_controlSignal_channel?Actuator_controlsignal_signal_parameter", 1, 2500, 0); /* m: 6 -> 85,0 */
	reached1[6] = 1;
	trans[1][6]	= settr(0,0,0,0,0,"Actuator_controlsignal_channel_used = 1",0,0,0);
	T = trans[ 1][85] = settr(98,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(98,0,83,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[1][83] = settr(96,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(96,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(96,2,44,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(96,2,81,0,0,"IF", 1, 2500, 0);
	trans[1][7]	= settr(20,2,11,171,0,"((global_state.actuator.state==1))", 1, 2500, 0);
	T = trans[ 1][11] = settr(24,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(24,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][8]	= settr(21,2,10,172,172,"global_state.actuator.inputcontrol = Actuator_controlsignal_signal_parameter", 1, 2500, 0);
	T = trans[ 1][10] = settr(23,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(23,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][9]	= settr(22,2,43,173,0,"assert(((global_state.actuator.inputcontrol>=-(1000))&&(global_state.actuator.inputcontrol<=1000)))", 1, 2500, 0);
	T = trans[ 1][43] = settr(56,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(56,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][12]	= settr(25,2,40,174,174,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,40 */
	reached1[13] = 1;
	trans[1][13]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[1][41]	= settr(54,2,40,1,0,".(goto)", 1, 2500, 0);
	T = trans[1][40] = settr(53,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(53,2,37,0,0,"DO", 1, 2500, 0);
	T = trans[1][37] = settr(50,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(50,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(50,2,16,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(50,2,20,0,0,"IF", 1, 2500, 0);
	trans[1][14]	= settr(27,2,89,175,175,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][15]	= settr(28,2,89,1,0,"goto :b2", 1, 2500, 0);
	trans[1][38]	= settr(51,2,39,1,0,".(goto)", 1, 2500, 0);
	trans[1][16]	= settr(29,2,39,176,176,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 39,0 */
	reached1[17] = 1;
	trans[1][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][18]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[1][19]	= settr(32,2,39,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][20]	= settr(33,2,22,177,177,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][22] = settr(35,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(35,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][21]	= settr(34,2,26,1,0,"(1)", 1, 2500, 0);
	T = trans[ 1][26] = settr(39,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(39,0,23,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][23]	= settr(36,2,25,178,178,"global_state.actuator.setting = (1.500000*global_state.actuator.inputcontrol)", 1, 2500, 0);
	T = trans[ 1][25] = settr(38,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(38,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][24]	= settr(37,2,35,179,0,"assert(((global_state.actuator.setting>=-(1000))&&(global_state.actuator.setting<=1000)))", 1, 2500, 0);
	T = trans[1][35] = settr(48,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(48,2,27,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(48,2,31,0,0,"IF", 1, 2500, 0);
	trans[1][27]	= settr(40,2,39,180,180,"(((global_state.actuator.setting>10.000000)==1))", 1, 2500, 0); /* m: 28 -> 39,0 */
	reached1[28] = 1;
	trans[1][28]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][29]	= settr(0,0,0,0,0,"global_state.actuator.state = 1",0,0,0);
	trans[1][30]	= settr(43,2,39,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][36]	= settr(49,2,39,1,0,".(goto)", 1, 2500, 0);
	trans[1][31]	= settr(44,2,39,181,181,"(((global_state.actuator.setting>10.000000)==0))", 1, 2500, 0); /* m: 32 -> 39,0 */
	reached1[32] = 1;
	trans[1][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][33]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[1][34]	= settr(47,2,39,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][39]	= settr(52,2,40,1,0,"(1)", 1, 2500, 0);
	trans[1][42]	= settr(55,2,89,1,0,"break", 1, 2500, 0);
	trans[1][84]	= settr(97,2,89,1,0,".(goto)", 1, 2500, 0);
	trans[1][44]	= settr(57,2,48,182,0,"((global_state.actuator.state==0))", 1, 2500, 0);
	T = trans[ 1][48] = settr(61,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(61,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][45]	= settr(58,2,47,183,183,"global_state.actuator.inputcontrol = Actuator_controlsignal_signal_parameter", 1, 2500, 0);
	T = trans[ 1][47] = settr(60,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(60,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][46]	= settr(59,2,80,184,0,"assert(((global_state.actuator.inputcontrol>=-(1000))&&(global_state.actuator.inputcontrol<=1000)))", 1, 2500, 0);
	T = trans[ 1][80] = settr(93,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(93,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][49]	= settr(62,2,77,185,185,"transition_id = 0", 1, 2500, 0); /* m: 50 -> 0,77 */
	reached1[50] = 1;
	trans[1][50]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[1][78]	= settr(91,2,77,1,0,".(goto)", 1, 2500, 0);
	T = trans[1][77] = settr(90,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(90,2,74,0,0,"DO", 1, 2500, 0);
	T = trans[1][74] = settr(87,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(87,2,51,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(87,2,53,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(87,2,57,0,0,"IF", 1, 2500, 0);
	trans[1][51]	= settr(64,2,89,186,186,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][52]	= settr(65,2,89,1,0,"goto :b3", 1, 2500, 0);
	trans[1][75]	= settr(88,2,76,1,0,".(goto)", 1, 2500, 0);
	trans[1][53]	= settr(66,2,76,187,187,"((transition_id==0))", 1, 2500, 0); /* m: 54 -> 76,0 */
	reached1[54] = 1;
	trans[1][54]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][55]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[1][56]	= settr(69,2,76,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][57]	= settr(70,2,59,188,188,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][59] = settr(72,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(72,0,58,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][58]	= settr(71,2,63,1,0,"(1)", 1, 2500, 0);
	T = trans[ 1][63] = settr(76,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(76,0,60,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][60]	= settr(73,2,62,189,189,"global_state.actuator.setting = (1.500000*global_state.actuator.inputcontrol)", 1, 2500, 0);
	T = trans[ 1][62] = settr(75,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(75,0,61,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][61]	= settr(74,2,72,190,0,"assert(((global_state.actuator.setting>=-(1000))&&(global_state.actuator.setting<=1000)))", 1, 2500, 0);
	T = trans[1][72] = settr(85,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(85,2,64,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(85,2,68,0,0,"IF", 1, 2500, 0);
	trans[1][64]	= settr(77,2,76,191,191,"(((global_state.actuator.setting>10.000000)==1))", 1, 2500, 0); /* m: 65 -> 76,0 */
	reached1[65] = 1;
	trans[1][65]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][66]	= settr(0,0,0,0,0,"global_state.actuator.state = 1",0,0,0);
	trans[1][67]	= settr(80,2,76,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][73]	= settr(86,2,76,1,0,".(goto)", 1, 2500, 0);
	trans[1][68]	= settr(81,2,76,192,192,"(((global_state.actuator.setting>10.000000)==0))", 1, 2500, 0); /* m: 69 -> 76,0 */
	reached1[69] = 1;
	trans[1][69]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][70]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[1][71]	= settr(84,2,76,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][76]	= settr(89,2,77,1,0,"(1)", 1, 2500, 0);
	trans[1][79]	= settr(92,2,89,1,0,"break", 1, 2500, 0);
	trans[1][81]	= settr(94,2,82,2,0,"else", 1, 2500, 0);
	trans[1][82]	= settr(95,2,89,1,0,"(1)", 1, 2500, 0);
	trans[1][86]	= settr(99,2,89,1,0,"goto Actuator_controlsignal_loop", 1, 2500, 0);
	trans[1][90]	= settr(103,2,91,1,0,".(goto)", 1, 2500, 0);
	trans[1][87]	= settr(100,2,88,193,0,"(empty(Actuator_controlSignal_channel))", 1, 2500, 0);
	trans[1][88]	= settr(101,2,91,194,194,"(1)", 1, 2500, 0); /* m: 90 -> 91,0 */
	reached1[90] = 1;
	trans[1][91]	= settr(104,0,93,195,195,"Actuator_lock!1", 1, 2500, 0);
	trans[1][95]	= settr(108,0,96,1,0,"break", 0, 2, 0);
	trans[1][96]	= settr(109,0,0,196,196,"-end-", 0, 3500, 0);

	/* proctype 0: timer_manager_proc */

	trans[0] = (Trans **) emalloc(15*sizeof(Trans *));

	trans[0][1]	= settr(0,0,11,197,0,"(inited)", 1, 2, 0);
	trans[0][12]	= settr(11,0,11,1,0,".(goto)", 0, 2, 0);
	T = trans[0][11] = settr(10,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(10,0,10,0,0,"DO", 0, 2, 0);
	T = trans[ 0][10] = settr(9,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(9,2,2,0,0,"ATOMIC", 1, 8, 0);
	trans[0][2]	= settr(1,2,8,1,0,"(1)", 1, 8, 0);
	T = trans[0][8] = settr(7,2,0,0,0,"IF", 1, 8, 0);
	T = T->nxt	= settr(7,2,3,0,0,"IF", 1, 8, 0);
	    T->nxt	= settr(7,2,6,0,0,"IF", 1, 8, 0);
	trans[0][3]	= settr(2,2,4,198,0,"(global_state.timers.sensor.trigger.timer_enabled)", 1, 8, 0);
	trans[0][4]	= settr(3,0,11,199,199,"Sensor_trigger_channel!0", 1, 8, 0); /* m: 5 -> 11,0 */
	reached0[5] = 1;
	trans[0][5]	= settr(0,0,0,0,0,"global_state.timers.sensor.trigger.timer_enabled = 0",0,0,0);
	trans[0][9]	= settr(8,0,11,200,200,".(goto)", 1, 8, 0);
	trans[0][6]	= settr(5,2,7,2,0,"else", 1, 8, 0);
	trans[0][7]	= settr(6,0,11,201,201,"(1)", 1, 8, 0); /* m: 9 -> 11,0 */
	reached0[9] = 1;
	trans[0][13]	= settr(12,0,14,1,0,"break", 0, 2, 0);
	trans[0][14]	= settr(13,0,0,202,202,"-end-", 0, 3500, 0);
	/* np_ demon: */
	trans[_NP_] = (Trans **) emalloc(3*sizeof(Trans *));
	T = trans[_NP_][0] = settr(9997,0,1,_T5,0,"(np_)", 1,2,0);
	    T->nxt	  = settr(9998,0,0,_T2,0,"(1)",   0,2,0);
	T = trans[_NP_][1] = settr(9999,0,1,_T5,0,"(np_)", 1,2,0);
}

Trans *
settr(	int t_id, int a, int b, int c, int d,
	char *t, int g, int tpe0, int tpe1)
{	Trans *tmp = (Trans *) emalloc(sizeof(Trans));

	tmp->atom  = a&(6|32);	/* only (2|8|32) have meaning */
	if (!g) tmp->atom |= 8;	/* no global references */
	tmp->st    = b;
	tmp->tpe[0] = tpe0;
	tmp->tpe[1] = tpe1;
	tmp->tp    = t;
	tmp->t_id  = t_id;
	tmp->forw  = c;
	tmp->back  = d;
	return tmp;
}

Trans *
cpytr(Trans *a)
{	Trans *tmp = (Trans *) emalloc(sizeof(Trans));

	int i;
	tmp->atom  = a->atom;
	tmp->st    = a->st;
#ifdef HAS_UNLESS
	tmp->e_trans = a->e_trans;
	for (i = 0; i < HAS_UNLESS; i++)
		tmp->escp[i] = a->escp[i];
#endif
	tmp->tpe[0] = a->tpe[0];
	tmp->tpe[1] = a->tpe[1];
	for (i = 0; i < 6; i++)
	{	tmp->qu[i] = a->qu[i];
		tmp->ty[i] = a->ty[i];
	}
	tmp->tp    = (char *) emalloc(strlen(a->tp)+1);
	strcpy(tmp->tp, a->tp);
	tmp->t_id  = a->t_id;
	tmp->forw  = a->forw;
	tmp->back  = a->back;
	return tmp;
}

#ifndef NOREDUCE
int
srinc_set(int n)
{	if (n <= 2) return LOCAL;
	if (n <= 2+  DELTA) return Q_FULL_F; /* 's' or nfull  */
	if (n <= 2+2*DELTA) return Q_EMPT_F; /* 'r' or nempty */
	if (n <= 2+3*DELTA) return Q_EMPT_T; /* empty */
	if (n <= 2+4*DELTA) return Q_FULL_T; /* full  */
	if (n ==   5*DELTA) return GLOBAL;
	if (n ==   6*DELTA) return TIMEOUT_F;
	if (n ==   7*DELTA) return ALPHA_F;
	Uerror("cannot happen srinc_class");
	return BAD;
}
int
srunc(int n, int m)
{	switch(m) {
	case Q_FULL_F: return n-2;
	case Q_EMPT_F: return n-2-DELTA;
	case Q_EMPT_T: return n-2-2*DELTA;
	case Q_FULL_T: return n-2-3*DELTA;
	case ALPHA_F:
	case TIMEOUT_F: return 257; /* non-zero, and > MAXQ */
	}
	Uerror("cannot happen srunc");
	return 0;
}
#endif
int cnt;
#ifdef HAS_UNLESS
int
isthere(Trans *a, int b)
{	Trans *t;
	for (t = a; t; t = t->nxt)
		if (t->t_id == b)
			return 1;
	return 0;
}
#endif
#ifndef NOREDUCE
int
mark_safety(Trans *t) /* for conditional safety */
{	int g = 0, i, j, k;

	if (!t) return 0;
	if (t->qu[0])
		return (t->qu[1])?2:1;	/* marked */

	for (i = 0; i < 2; i++)
	{	j = srinc_set(t->tpe[i]);
		if (j >= GLOBAL && j != ALPHA_F)
			return -1;
		if (j != LOCAL)
		{	k = srunc(t->tpe[i], j);
			if (g == 0
			||  t->qu[0] != k
			||  t->ty[0] != j)
			{	t->qu[g] = k;
				t->ty[g] = j;
				g++;
	}	}	}
	return g;
}
#endif
void
retrans(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
	/* process n, with m states, is=initial state */
{	Trans *T0, *T1, *T2, *T3;
	Trans *T4, *T5; /* t_reverse or has_unless */
	int i;
#if defined(HAS_UNLESS) || !defined(NOREDUCE)
	int k;
#endif
#ifndef NOREDUCE
	int g, h, j, aa;
#endif
#ifdef HAS_UNLESS
	int p;
#endif
	if (state_tables >= 4)
	{	printf("STEP 1 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
	do {
		for (i = 1, cnt = 0; i < m; i++)
		{	T2 = trans[n][i];
			T1 = T2?T2->nxt:(Trans *)0;
/* prescan: */		for (T0 = T1; T0; T0 = T0->nxt)
/* choice in choice */	{	if (T0->st && trans[n][T0->st]
				&&  trans[n][T0->st]->nxt)
					break;
			}
#if 0
		if (T0)
		printf("\tstate %d / %d: choice in choice\n",
		i, T0->st);
#endif
			if (T0)
			for (T0 = T1; T0; T0 = T0->nxt)
			{	T3 = trans[n][T0->st];
				if (!T3->nxt)
				{	T2->nxt = cpytr(T0);
					T2 = T2->nxt;
					imed(T2, T0->st, n, i);
					continue;
				}
				do {	T3 = T3->nxt;
					T2->nxt = cpytr(T3);
					T2 = T2->nxt;
					imed(T2, T0->st, n, i);
				} while (T3->nxt);
				cnt++;
			}
		}
	} while (cnt);
	if (state_tables >= 3)
	{	printf("STEP 2 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
	for (i = 1; i < m; i++)
	{	if (trans[n][i] && trans[n][i]->nxt) /* optimize */
		{	T1 = trans[n][i]->nxt;
#if 0
			printf("\t\tpull %d (%d) to %d\n",
			T1->st, T1->forw, i);
#endif
			srcln[i] = srcln[T1->st];	/* Oyvind Teig, 5.2.0 */

			if (!trans[n][T1->st]) continue;
			T0 = cpytr(trans[n][T1->st]);
			trans[n][i] = T0;
			reach[T1->st] = 1;
			imed(T0, T1->st, n, i);
			for (T1 = T1->nxt; T1; T1 = T1->nxt)
			{
#if 0
			printf("\t\tpull %d (%d) to %d\n",
				T1->st, T1->forw, i);
#endif
		/*		srcln[i] = srcln[T1->st];  gh: not useful */
				if (!trans[n][T1->st]) continue;
				T0->nxt = cpytr(trans[n][T1->st]);
				T0 = T0->nxt;
				reach[T1->st] = 1;
				imed(T0, T1->st, n, i);
	}	}	}
	if (state_tables >= 2)
	{	printf("STEP 3 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
#ifdef HAS_UNLESS
	for (i = 1; i < m; i++)
	{	if (!trans[n][i]) continue;
		/* check for each state i if an
		 * escape to some state p is defined
		 * if so, copy and mark p's transitions
		 * and prepend them to the transition-
		 * list of state i
		 */
	 if (!like_java) /* the default */
	 {	for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		for (k = HAS_UNLESS-1; k >= 0; k--)
		{	if (p = T0->escp[k])
			for (T1 = trans[n][p]; T1; T1 = T1->nxt)
			{	if (isthere(trans[n][i], T1->t_id))
					continue;
				T2 = cpytr(T1);
				T2->e_trans = p;
				T2->nxt = trans[n][i];
				trans[n][i] = T2;
		}	}
	 } else /* outermost unless checked first */
	 {	T4 = T3 = (Trans *) 0;
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		for (k = HAS_UNLESS-1; k >= 0; k--)
		{	if (p = T0->escp[k])
			for (T1 = trans[n][p]; T1; T1 = T1->nxt)
			{	if (isthere(trans[n][i], T1->t_id))
					continue;
				T2 = cpytr(T1);
				T2->nxt = (Trans *) 0;
				T2->e_trans = p;
				if (T3)	T3->nxt = T2;
				else	T4 = T2;
				T3 = T2;
		}	}
		if (T4)
		{	T3->nxt = trans[n][i];
			trans[n][i] = T4;
		}
	 }
	}
#endif
#ifndef NOREDUCE
	for (i = 1; i < m; i++)
	{	if (a_cycles)
		{ /* moves through these states are visible */
	#if PROG_LAB>0 && defined(HAS_NP)
			if (progstate[n][i])
				goto degrade;
			for (T1 = trans[n][i]; T1; T1 = T1->nxt)
				if (progstate[n][T1->st])
					goto degrade;
	#endif
			if (accpstate[n][i] || visstate[n][i])
				goto degrade;
			for (T1 = trans[n][i]; T1; T1 = T1->nxt)
				if (accpstate[n][T1->st])
					goto degrade;
		}
		T1 = trans[n][i];
		if (!T1) continue;
		g = mark_safety(T1);	/* V3.3.1 */
		if (g < 0) goto degrade; /* global */
		/* check if mixing of guards preserves reduction */
		if (T1->nxt)
		{	k = 0;
			for (T0 = T1; T0; T0 = T0->nxt)
			{	if (!(T0->atom&8))
					goto degrade;
				for (aa = 0; aa < 2; aa++)
				{	j = srinc_set(T0->tpe[aa]);
					if (j >= GLOBAL && j != ALPHA_F)
						goto degrade;
					if (T0->tpe[aa]
					&&  T0->tpe[aa]
					!=  T1->tpe[0])
						k = 1;
			}	}
			/* g = 0;	V3.3.1 */
			if (k)	/* non-uniform selection */
			for (T0 = T1; T0; T0 = T0->nxt)
			for (aa = 0; aa < 2; aa++)
			{	j = srinc_set(T0->tpe[aa]);
				if (j != LOCAL)
				{	k = srunc(T0->tpe[aa], j);
					for (h = 0; h < 6; h++)
						if (T1->qu[h] == k
						&&  T1->ty[h] == j)
							break;
					if (h >= 6)
					{	T1->qu[g%6] = k;
						T1->ty[g%6] = j;
						g++;
			}	}	}
			if (g > 6)
			{	T1->qu[0] = 0;	/* turn it off */
				printf("pan: warning, line %d, ",
					srcln[i]);
			 	printf("too many stmnt types (%d)",
					g);
			  	printf(" in selection\n");
			  goto degrade;
			}
		}
		/* mark all options global if >=1 is global */
		for (T1 = trans[n][i]; T1; T1 = T1->nxt)
			if (!(T1->atom&8)) break;
		if (T1)
degrade:	for (T1 = trans[n][i]; T1; T1 = T1->nxt)
			T1->atom &= ~8;	/* mark as unsafe */
		/* can only mix 'r's or 's's if on same chan */
		/* and not mixed with other local operations */
		T1 = trans[n][i];
		if (!T1 || T1->qu[0]) continue;
		j = T1->tpe[0];
		if (T1->nxt && T1->atom&8)
		{ if (j == 5*DELTA)
		  {	printf("warning: line %d ", srcln[i]);
			printf("mixed condition ");
			printf("(defeats reduction)\n");
			goto degrade;
		  }
		  for (T0 = T1; T0; T0 = T0->nxt)
		  for (aa = 0; aa < 2; aa++)
		  if  (T0->tpe[aa] && T0->tpe[aa] != j)
		  {	printf("warning: line %d ", srcln[i]);
			printf("[%d-%d] mixed %stion ",
				T0->tpe[aa], j, 
				(j==5*DELTA)?"condi":"selec");
			printf("(defeats reduction)\n");
			printf("	'%s' <-> '%s'\n",
				T1->tp, T0->tp);
			goto degrade;
		} }
	}
#endif
	for (i = 1; i < m; i++)
	{	T2 = trans[n][i];
		if (!T2
		||  T2->nxt
		||  strncmp(T2->tp, ".(goto)", 7)
		||  !stopstate[n][i])
			continue;
		stopstate[n][T2->st] = 1;
	}
	if (state_tables && !verbose)
	{	if (dodot)
		{	char buf[256], *q = buf, *p = procname[n];
			while (*p != '\0')
			{	if (*p != ':')
				{	*q++ = *p;
				}
				p++;
			}
			*q = '\0';
			printf("digraph ");
			switch (Btypes[n]) {
			case I_PROC:  printf("init {\n"); break;
			case N_CLAIM: printf("claim_%s {\n", buf); break;
			case E_TRACE: printf("notrace {\n"); break;
			case N_TRACE: printf("trace {\n"); break;
			default:      printf("p_%s {\n", buf); break;
			}
			printf("size=\"8,10\";\n");
			printf("  GT [shape=box,style=dotted,label=\"%s\"];\n", buf);
			printf("  GT -> S%d;\n", is);
		} else
		{	switch (Btypes[n]) {
			case I_PROC:  printf("init\n"); break;
			case N_CLAIM: printf("claim %s\n", procname[n]); break;
			case E_TRACE: printf("notrace assertion\n"); break;
			case N_TRACE: printf("trace assertion\n"); break;
			default:      printf("proctype %s\n", procname[n]); break;
		}	}
		for (i = 1; i < m; i++)
		{	reach[i] = 1;
		}
		tagtable(n, m, is, srcln, reach);
		if (dodot) printf("}\n");
	} else
	for (i = 1; i < m; i++)
	{	int nrelse;
		if (Btypes[n] != N_CLAIM)
		{	for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			{	if (T0->st == i
				&& strcmp(T0->tp, "(1)") == 0)
				{	printf("error: proctype '%s' ",
						procname[n]);
		  			printf("line %d, state %d: has un",
						srcln[i], i);
					printf("conditional self-loop\n");
					pan_exit(1);
		}	}	}
		nrelse = 0;
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		{	if (strcmp(T0->tp, "else") == 0)
				nrelse++;
		}
		if (nrelse > 1)
		{	printf("error: proctype '%s' state",
				procname[n]);
		  	printf(" %d, inherits %d", i, nrelse);
		  	printf(" 'else' stmnts\n");
			pan_exit(1);
	}	}
#if !defined(LOOPSTATE) && !defined(BFS_PAR)
	if (state_tables)
#endif
	do_dfs(n, m, is, srcln, reach, lpstate);

	if (!t_reverse)
	{	return;
	}
	/* process n, with m states, is=initial state -- reverse list */
	if (!state_tables && Btypes[n] != N_CLAIM)
	{	for (i = 1; i < m; i++)
		{	Trans *Tx = (Trans *) 0; /* list of escapes */
			Trans *Ty = (Trans *) 0; /* its tail element */
			T1 = (Trans *) 0; /* reversed list */
			T2 = (Trans *) 0; /* its tail */
			T3 = (Trans *) 0; /* remembers possible 'else' */

			/* find unless-escapes, they should go first */
			T4 = T5 = T0 = trans[n][i];
	#ifdef HAS_UNLESS
			while (T4 && T4->e_trans) /* escapes are first in orig list */
			{	T5 = T4;	  /* remember predecessor */
				T4 = T4->nxt;
			}
	#endif
			/* T4 points to first non-escape, T5 to its parent, T0 to original list */
			if (T4 != T0)		 /* there was at least one escape */
			{	T3 = T5->nxt;		 /* start of non-escapes */
				T5->nxt = (Trans *) 0;	 /* separate */
				Tx = T0;		 /* start of the escapes */
				Ty = T5;		 /* its tail */
				T0 = T3;		 /* the rest, to be reversed */
			}
			/* T0 points to first non-escape, Tx to the list of escapes, Ty to its tail */

			/* first tail-add non-escape transitions, reversed */
			T3 = (Trans *) 0;
			for (T5 = T0; T5; T5 = T4)
			{	T4 = T5->nxt;
	#ifdef HAS_UNLESS
				if (T5->e_trans)
				{	printf("error: cannot happen!\n");
					continue;
				}
	#endif
				if (strcmp(T5->tp, "else") == 0)
				{	T3 = T5;
					T5->nxt = (Trans *) 0;
				} else
				{	T5->nxt = T1;
					if (!T1) { T2 = T5; }
					T1 = T5;
			}	}
			/* T3 points to a possible else, which is removed from the list */
			/* T1 points to the reversed list so far (without escapes) */
			/* T2 points to the tail element -- where the else should go */
			if (T2 && T3)
			{	T2->nxt = T3;	/* add else */
			} else
			{	if (T3) /* there was an else, but there's no tail */
				{	if (!T1)	/* and no reversed list */
					{	T1 = T3; /* odd, but possible */
					} else		/* even stranger */
					{	T1->nxt = T3;
			}	}	}

			/* add in the escapes, to that they appear at the front */
			if (Tx && Ty) { Ty->nxt = T1; T1 = Tx; }

			trans[n][i] = T1;
			/* reversed, with escapes first and else last */
	}	}
	if (state_tables && verbose)
	{	printf("FINAL proctype %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
	}
}
void
imed(Trans *T, int v, int n, int j)	/* set intermediate state */
{	progstate[n][T->st] |= progstate[n][v];
	accpstate[n][T->st] |= accpstate[n][v];
	stopstate[n][T->st] |= stopstate[n][v];
	mapstate[n][j] = T->st;
}
void
tagtable(int n, int m, int is, short srcln[], uchar reach[])
{	Trans *z;

	if (is >= m || !trans[n][is]
	||  is <= 0 || reach[is] == 0)
		return;
	reach[is] = 0;
	if (state_tables)
	for (z = trans[n][is]; z; z = z->nxt)
	{	if (dodot)
			dot_crack(n, is, z);
		else
			crack(n, is, z, srcln);
	}

	for (z = trans[n][is]; z; z = z->nxt)
	{
#ifdef HAS_UNLESS
		int i, j;
#endif
		tagtable(n, m, z->st, srcln, reach);
#ifdef HAS_UNLESS
		for (i = 0; i < HAS_UNLESS; i++)
		{	j = trans[n][is]->escp[i];
			if (!j) break;
			tagtable(n, m, j, srcln, reach);
		}
#endif
	}
}

extern Trans *t_id_lkup[];

void
dfs_table(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
{	Trans *z;

	if (is >= m || is <= 0 || !trans[n][is])
		return;
	if ((reach[is] & (4|8|16)) != 0)
	{	if ((reach[is] & (8|16)) == 16)	/* on stack, not yet recorded */
		{	lpstate[is] = 1;
			reach[is] |= 8; /* recorded */
			if (state_tables && verbose)
			{	printf("state %d line %d is a loopstate\n", is, srcln[is]);
		}	}
		return;
	}
	reach[is] |= (4|16);	/* visited | onstack */
	for (z = trans[n][is]; z; z = z->nxt)
	{	t_id_lkup[z->t_id] = z;
#ifdef HAS_UNLESS
		int i, j;
#endif
		dfs_table(n, m, z->st, srcln, reach, lpstate);
#ifdef HAS_UNLESS
		for (i = 0; i < HAS_UNLESS; i++)
		{	j = trans[n][is]->escp[i];
			if (!j) break;
			dfs_table(n, m, j, srcln, reach, lpstate);
		}
#endif
	}
	reach[is] &= ~16; /* no longer on stack */
}
void
do_dfs(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
{	int i;
	dfs_table(n, m, is, srcln, reach, lpstate);
	for (i = 0; i < m; i++)
		reach[i] &= ~(4|8|16);
}
void
crack(int n, int j, Trans *z, short srcln[])
{	int i;

	if (!z) return;
	printf("	state %3d -(tr %3d)-> state %3d  ",
		j, z->forw, z->st);
	printf("[id %3d tp %3d", z->t_id, z->tpe[0]);
	if (z->tpe[1]) printf(",%d", z->tpe[1]);
#ifdef HAS_UNLESS
	if (z->e_trans)
		printf(" org %3d", z->e_trans);
	else if (state_tables >= 2)
	for (i = 0; i < HAS_UNLESS; i++)
	{	if (!z->escp[i]) break;
		printf(" esc %d", z->escp[i]);
	}
#endif
	printf("]");
	printf(" [%s%s%s%s%s] %s:%d => ",
		z->atom&6?"A":z->atom&32?"D":"-",
		accpstate[n][j]?"a" :"-",
		stopstate[n][j]?"e" : "-",
		progstate[n][j]?"p" : "-",
		z->atom & 8 ?"L":"G",
		PanSource, srcln[j]);
	for (i = 0; z->tp[i]; i++)
		if (z->tp[i] == '\n')
			printf("\\n");
		else
			putchar(z->tp[i]);
	if (verbose && z->qu[0])
	{	printf("\t[");
		for (i = 0; i < 6; i++)
			if (z->qu[i])
				printf("(%d,%d)",
				z->qu[i], z->ty[i]);
		printf("]");
	}
	printf("\n");
	fflush(stdout);
}
/* spin -a m.pml; cc -o pan pan.c; ./pan -D | dot -Tps > foo.ps; ps2pdf foo.ps */
void
dot_crack(int n, int j, Trans *z)
{	int i;

	if (!z) return;
	printf("	S%d -> S%d  [color=black", j, z->st);

	if (z->atom&6) printf(",style=dashed");
	else if (z->atom&32) printf(",style=dotted");
	else if (z->atom&8) printf(",style=solid");
	else printf(",style=bold");

	printf(",label=\"");
	for (i = 0; z->tp[i]; i++)
	{	if (z->tp[i] == '\\'
		&&  z->tp[i+1] == 'n')
		{	i++; printf(" ");
		} else
		{	putchar(z->tp[i]);
	}	}
	printf("\"];\n");
	if (accpstate[n][j]) printf("  S%d [color=red,style=bold];\n", j);
	else if (progstate[n][j]) printf("  S%d [color=green,style=bold];\n", j);
	if (stopstate[n][j]) printf("  S%d [color=blue,style=bold,shape=box];\n", j);
}

#ifdef VAR_RANGES
#define BYTESIZE	32	/* 2^8 : 2^3 = 256:8 = 32 */

typedef struct Vr_Ptr {
	char	*nm;
	uchar	vals[BYTESIZE];
	struct Vr_Ptr *nxt;
} Vr_Ptr;
Vr_Ptr *ranges = (Vr_Ptr *) 0;

void
logval(char *s, int v)
{	Vr_Ptr *tmp;

	if (v<0 || v > 255) return;
	for (tmp = ranges; tmp; tmp = tmp->nxt)
		if (!strcmp(tmp->nm, s))
			goto found;
	tmp = (Vr_Ptr *) emalloc(sizeof(Vr_Ptr));
	tmp->nxt = ranges;
	ranges = tmp;
	tmp->nm = s;
found:
	tmp->vals[(v)/8] |= 1<<((v)%8);
}

void
dumpval(uchar X[], int range)
{	int w, x, i, j = -1;

	for (w = i = 0; w < range; w++)
	for (x = 0; x < 8; x++, i++)
	{
from:		if ((X[w] & (1<<x)))
		{	printf("%d", i);
			j = i;
			goto upto;
	}	}
	return;
	for (w = 0; w < range; w++)
	for (x = 0; x < 8; x++, i++)
	{
upto:		if (!(X[w] & (1<<x)))
		{	if (i-1 == j)
				printf(", ");
			else
				printf("-%d, ", i-1);
			goto from;
	}	}
	if (j >= 0 && j != 255)
		printf("-255");
}

void
dumpranges(void)
{	Vr_Ptr *tmp;
	printf("\nValues assigned within ");
	printf("interval [0..255]:\n");
	for (tmp = ranges; tmp; tmp = tmp->nxt)
	{	printf("\t%s\t: ", tmp->nm);
		dumpval(tmp->vals, BYTESIZE);
		printf("\n");
	}
}
#endif
