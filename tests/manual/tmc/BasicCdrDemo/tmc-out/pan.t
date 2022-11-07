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

	trans = (Trans ***) emalloc(10*sizeof(Trans **));

	/* proctype 8: never_0 */

	trans[8] = (Trans **) emalloc(18*sizeof(Trans *));

	trans[8][6]	= settr(580,0,5,1,0,".(goto)", 0, 2, 0);
	T = trans[8][5] = settr(579,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(579,0,1,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(579,0,3,0,0,"DO", 0, 2, 0);
	trans[8][1]	= settr(575,0,14,3,0,"(inited)", 1, 2, 0);
	trans[8][2]	= settr(576,0,14,1,0,"goto :b23", 0, 2, 0);
	trans[8][3]	= settr(577,0,5,2,0,"else", 0, 2, 0);
	trans[8][4]	= settr(578,0,5,1,0,"goto start", 0, 2, 0);
	trans[8][7]	= settr(581,0,14,1,0,"break", 0, 2, 0);
	trans[8][15]	= settr(589,0,14,1,0,".(goto)", 0, 2, 0);
	T = trans[8][14] = settr(588,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(588,0,11,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(588,0,12,0,0,"DO", 0, 2, 0);
	T = trans[ 8][11] = settr(585,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(585,2,8,0,0,"ATOMIC", 1, 2, 0);
	trans[8][8]	= settr(582,0,14,4,4,"((global_state.actuator.x>5))", 1, 2, 0); /* m: 9 -> 14,0 */
	reached8[9] = 1;
	trans[8][9]	= settr(0,0,0,0,0,"printf('SCL violation: never actuator.x  > 5;\\n')",0,0,0);
	trans[8][10]	= settr(0,0,0,0,0,"assert(!((global_state.actuator.x>5)))",0,0,0);
	trans[8][12]	= settr(586,0,14,2,0,"else", 0, 2, 0);
	trans[8][13]	= settr(587,0,14,1,0,"goto system_inited", 0, 2, 0);
	trans[8][16]	= settr(590,0,17,1,0,"break", 0, 2, 0);
	trans[8][17]	= settr(591,0,0,5,5,"-end-", 0, 3500, 0);

	/* proctype 7: :init: */

	trans[7] = (Trans **) emalloc(105*sizeof(Trans *));

	T = trans[ 7][103] = settr(573,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(573,2,5,0,0,"ATOMIC", 1, 8, 9);
	T = trans[ 7][5] = settr(475,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(475,0,4,0,0,"sub-sequence", 1, 8, 9);
/*->*/	trans[7][4]	= settr(474,34,42,6,6,"D_STEP12361", 1, 2, 0);
	T = trans[ 7][42] = settr(512,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(512,0,9,0,0,"sub-sequence", 1, 8, 9);
	T = trans[ 7][9] = settr(479,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(479,0,6,0,0,"sub-sequence", 1, 8, 9);
	trans[7][6]	= settr(476,2,8,7,7,"global_state.actuator.sender = 4", 1, 8, 9);
	T = trans[ 7][8] = settr(478,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(478,0,7,0,0,"sub-sequence", 1, 8, 9);
	trans[7][7]	= settr(477,2,41,8,0,"assert((((((global_state.actuator.sender==0)||(global_state.actuator.sender==1))||(global_state.actuator.sender==2))||(global_state.actuator.sender==3))||(global_state.actuator.sender==4)))", 1, 8, 9);
	T = trans[ 7][41] = settr(511,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(511,0,10,0,0,"sub-sequence", 1, 8, 9);
	trans[7][10]	= settr(480,2,38,9,9,"transition_id = 0", 1, 8, 9); /* m: 11 -> 0,38 */
	reached7[11] = 1;
	trans[7][11]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[7][39]	= settr(509,2,38,1,0,".(goto)", 1, 8, 9);
	T = trans[7][38] = settr(508,2,0,0,0,"DO", 1, 8, 9);
	    T->nxt	= settr(508,2,35,0,0,"DO", 1, 8, 9);
	T = trans[7][35] = settr(505,2,0,0,0,"IF", 1, 8, 9);
	T = T->nxt	= settr(505,2,12,0,0,"IF", 1, 8, 9);
	T = T->nxt	= settr(505,2,14,0,0,"IF", 1, 8, 9);
	    T->nxt	= settr(505,2,18,0,0,"IF", 1, 8, 9);
	trans[7][12]	= settr(482,2,43,10,10,"((transition_id==-(1)))", 1, 8, 9);
	trans[7][13]	= settr(483,2,43,1,0,"goto :b21", 1, 8, 9);
	trans[7][36]	= settr(506,2,37,1,0,".(goto)", 1, 8, 9);
	trans[7][14]	= settr(484,2,37,11,11,"((transition_id==0))", 1, 8, 9); /* m: 15 -> 37,0 */
	reached7[15] = 1;
	trans[7][15]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][16]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[7][17]	= settr(487,2,37,1,0,"goto continuous_signals", 1, 8, 9);
	trans[7][18]	= settr(488,2,20,12,12,"((transition_id==1))", 1, 8, 9);
	T = trans[ 7][20] = settr(490,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(490,0,19,0,0,"sub-sequence", 1, 8, 9);
	trans[7][19]	= settr(489,2,33,1,0,"(1)", 1, 8, 9);
	T = trans[7][33] = settr(503,2,0,0,0,"IF", 1, 8, 9);
	T = T->nxt	= settr(503,2,21,0,0,"IF", 1, 8, 9);
	    T->nxt	= settr(503,2,27,0,0,"IF", 1, 8, 9);
	trans[7][21]	= settr(491,2,23,13,0,"(((global_state.actuator.x>10)==1))", 1, 8, 9);
	T = trans[ 7][23] = settr(493,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(493,0,22,0,0,"sub-sequence", 1, 8, 9);
	trans[7][22]	= settr(492,2,37,14,14,"Modemanager_feedback_channel!global_state.actuator.x", 1, 8, 9); /* m: 24 -> 37,0 */
	reached7[24] = 1;
	trans[7][24]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][25]	= settr(0,0,0,0,0,"global_state.actuator.state = 1",0,0,0);
	trans[7][26]	= settr(496,2,37,1,0,"goto continuous_signals", 1, 8, 9);
	trans[7][34]	= settr(504,2,37,1,0,".(goto)", 1, 8, 9);
	trans[7][27]	= settr(497,2,29,15,0,"(((global_state.actuator.x>10)==0))", 1, 8, 9);
	T = trans[ 7][29] = settr(499,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(499,0,28,0,0,"sub-sequence", 1, 8, 9);
	trans[7][28]	= settr(498,2,37,16,16,"Modemanager_feedback_channel!0", 1, 8, 9); /* m: 30 -> 37,0 */
	reached7[30] = 1;
	trans[7][30]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][31]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[7][32]	= settr(502,2,37,1,0,"goto continuous_signals", 1, 8, 9);
	trans[7][37]	= settr(507,2,38,1,0,"(1)", 1, 8, 9);
	trans[7][40]	= settr(510,2,43,1,0,"break", 1, 8, 9);
	trans[7][43]	= settr(513,2,100,17,17,"Actuator_lock!1", 1, 8, 9);
	T = trans[ 7][100] = settr(570,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(570,0,47,0,0,"sub-sequence", 1, 8, 9);
	T = trans[ 7][47] = settr(517,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(517,0,44,0,0,"sub-sequence", 1, 8, 9);
	trans[7][44]	= settr(514,2,46,18,18,"global_state.modemanager.sender = 4", 1, 8, 9);
	T = trans[ 7][46] = settr(516,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(516,0,45,0,0,"sub-sequence", 1, 8, 9);
	trans[7][45]	= settr(515,2,99,19,0,"assert((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4)))", 1, 8, 9);
	T = trans[ 7][99] = settr(569,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(569,0,48,0,0,"sub-sequence", 1, 8, 9);
	trans[7][48]	= settr(518,2,96,20,20,"transition_id = 0", 1, 8, 9); /* m: 49 -> 0,96 */
	reached7[49] = 1;
	trans[7][49]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[7][97]	= settr(567,2,96,1,0,".(goto)", 1, 8, 9);
	T = trans[7][96] = settr(566,2,0,0,0,"DO", 1, 8, 9);
	    T->nxt	= settr(566,2,93,0,0,"DO", 1, 8, 9);
	T = trans[7][93] = settr(563,2,0,0,0,"IF", 1, 8, 9);
	T = T->nxt	= settr(563,2,50,0,0,"IF", 1, 8, 9);
	T = T->nxt	= settr(563,2,52,0,0,"IF", 1, 8, 9);
	T = T->nxt	= settr(563,2,56,0,0,"IF", 1, 8, 9);
	T = T->nxt	= settr(563,2,76,0,0,"IF", 1, 8, 9);
	    T->nxt	= settr(563,2,87,0,0,"IF", 1, 8, 9);
	trans[7][50]	= settr(520,2,101,21,21,"((transition_id==-(1)))", 1, 8, 9);
	trans[7][51]	= settr(521,2,101,1,0,"goto :b22", 1, 8, 9);
	trans[7][94]	= settr(564,2,95,1,0,".(goto)", 1, 8, 9);
	trans[7][52]	= settr(522,2,95,22,22,"((transition_id==0))", 1, 8, 9); /* m: 53 -> 95,0 */
	reached7[53] = 1;
	trans[7][53]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][54]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[7][55]	= settr(525,2,95,1,0,"goto continuous_signals", 1, 8, 9);
	trans[7][56]	= settr(526,2,58,23,23,"((transition_id==1))", 1, 8, 9);
	T = trans[ 7][58] = settr(528,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(528,0,57,0,0,"sub-sequence", 1, 8, 9);
	trans[7][57]	= settr(527,2,71,1,0,"(1)", 1, 8, 9);
	T = trans[ 7][71] = settr(541,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(541,0,62,0,0,"sub-sequence", 1, 8, 9);
	T = trans[ 7][62] = settr(532,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(532,0,59,0,0,"sub-sequence", 1, 8, 9);
	trans[7][59]	= settr(529,2,61,24,24,"global_state.modemanager.rprt.reading = global_state.modemanager.xin", 1, 8, 9);
	T = trans[ 7][61] = settr(531,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(531,0,60,0,0,"sub-sequence", 1, 8, 9);
	trans[7][60]	= settr(530,2,66,25,0,"assert(((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=1000000)))", 1, 8, 9);
	T = trans[ 7][66] = settr(536,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(536,0,63,0,0,"sub-sequence", 1, 8, 9);
	trans[7][63]	= settr(533,2,65,26,26,"global_state.modemanager.rprt.ctrl = global_state.modemanager.xout", 1, 8, 9);
	T = trans[ 7][65] = settr(535,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(535,0,64,0,0,"sub-sequence", 1, 8, 9);
	trans[7][64]	= settr(534,2,70,27,0,"assert(((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=1000000)))", 1, 8, 9);
	T = trans[ 7][70] = settr(540,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(540,0,67,0,0,"sub-sequence", 1, 8, 9);
	trans[7][67]	= settr(537,2,69,28,28,"global_state.modemanager.rprt.feedback = global_state.modemanager.xfeedback", 1, 8, 9);
	T = trans[ 7][69] = settr(539,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(539,0,68,0,0,"sub-sequence", 1, 8, 9);
	trans[7][68]	= settr(538,2,73,29,0,"assert(((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=1000000)))", 1, 8, 9);
	T = trans[ 7][73] = settr(543,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(543,0,72,0,0,"sub-sequence", 1, 8, 9);
	trans[7][72]	= settr(542,2,95,30,30,"Egse_tm_channel!global_state.modemanager.rprt.reading,global_state.modemanager.rprt.ctrl,global_state.modemanager.rprt.feedback", 1, 8, 9); /* m: 74 -> 95,0 */
	reached7[74] = 1;
	trans[7][74]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][75]	= settr(545,2,95,1,0,"goto continuous_signals", 1, 8, 9);
	trans[7][76]	= settr(546,2,78,31,31,"((transition_id==2))", 1, 8, 9);
	T = trans[ 7][78] = settr(548,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(548,0,77,0,0,"sub-sequence", 1, 8, 9);
	trans[7][77]	= settr(547,2,82,1,0,"(1)", 1, 8, 9);
	T = trans[ 7][82] = settr(552,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(552,0,79,0,0,"sub-sequence", 1, 8, 9);
	trans[7][79]	= settr(549,2,81,32,32,"global_state.modemanager.xout = ((global_state.modemanager.xin*global_state.modemanager.cfg.param_a)+global_state.modemanager.cfg.param_b)", 1, 8, 9);
	T = trans[ 7][81] = settr(551,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(551,0,80,0,0,"sub-sequence", 1, 8, 9);
	trans[7][80]	= settr(550,2,84,33,0,"assert(((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=1000000)))", 1, 8, 9);
	T = trans[ 7][84] = settr(554,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(554,0,83,0,0,"sub-sequence", 1, 8, 9);
	trans[7][83]	= settr(553,2,95,34,34,"Actuator_ctrl_channel!global_state.modemanager.xout", 1, 8, 9); /* m: 85 -> 95,0 */
	reached7[85] = 1;
	trans[7][85]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][86]	= settr(556,2,95,1,0,"goto continuous_signals", 1, 8, 9);
	trans[7][87]	= settr(557,2,89,35,35,"((transition_id==3))", 1, 8, 9);
	T = trans[ 7][89] = settr(559,0,0,0,0,"sub-sequence", 1, 8, 9);
	T->nxt	= settr(559,0,88,0,0,"sub-sequence", 1, 8, 9);
	trans[7][88]	= settr(558,2,95,36,36,"(1)", 1, 8, 9); /* m: 90 -> 95,0 */
	reached7[90] = 1;
	trans[7][90]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][91]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[7][92]	= settr(562,2,95,1,0,"goto continuous_signals", 1, 8, 9);
	trans[7][95]	= settr(565,2,96,1,0,"(1)", 1, 8, 9);
	trans[7][98]	= settr(568,2,101,1,0,"break", 1, 8, 9);
	trans[7][101]	= settr(571,4,104,37,37,"Modemanager_lock!1", 1, 8, 9); /* m: 102 -> 104,0 */
	reached7[102] = 1;
	trans[7][102]	= settr(0,0,0,0,0,"inited = 1",0,0,0);
	trans[7][104]	= settr(574,0,0,38,38,"-end-", 0, 3500, 0);

	/* proctype 6: Sensor_reading */

	trans[6] = (Trans **) emalloc(45*sizeof(Trans *));

	trans[6][1]	= settr(427,0,2,39,0,"(inited)", 1, 2, 0);
	trans[6][2]	= settr(428,0,41,40,40,"value = 0", 0, 2, 0); /* m: 3 -> 0,41 */
	reached6[3] = 1;
	trans[6][3]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[6][4]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[6][42]	= settr(468,0,41,1,0,".(goto)", 0, 2, 0);
	T = trans[6][41] = settr(467,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(467,0,5,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(467,0,39,0,0,"DO", 0, 2, 0);
	trans[6][5]	= settr(431,0,37,41,0,"((inputVectorCounter<=3))", 0, 2, 0);
	T = trans[ 6][37] = settr(463,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(463,2,34,0,0,"ATOMIC", 1, 2, 0);
	T = trans[ 6][34] = settr(460,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(460,0,6,0,0,"sub-sequence", 1, 2, 0);
	trans[6][6]	= settr(432,2,31,42,42,"Sensor_reading_x_tmp = 0", 1, 2, 0);
	T = trans[6][31] = settr(457,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(457,2,14,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(457,2,22,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(457,2,30,0,0,"IF", 1, 2, 0);
	T = trans[ 6][14] = settr(440,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(440,0,7,0,0,"sub-sequence", 1, 2, 0);
	trans[6][7]	= settr(433,2,11,43,43,"Sensor_reading_x_tmp = 0", 1, 2, 0);
	trans[6][12]	= settr(438,2,11,1,0,".(goto)", 1, 2, 0);
	T = trans[6][11] = settr(437,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(437,2,10,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(437,2,8,0,0,"DO", 1, 2, 0);
	trans[6][10]	= settr(436,2,36,44,44,"goto :b18", 1, 2, 0); /* m: 33 -> 0,36 */
	reached6[33] = 1;
	trans[6][8]	= settr(434,2,11,45,45,"((Sensor_reading_x_tmp<0))", 1, 2, 0); /* m: 9 -> 11,0 */
	reached6[9] = 1;
	trans[6][9]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[6][13]	= settr(439,2,33,1,0,"break", 1, 2, 0);
	trans[6][32]	= settr(458,2,33,1,0,".(goto)", 1, 2, 0); /* m: 33 -> 0,36 */
	reached6[33] = 1;
	T = trans[ 6][22] = settr(448,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(448,0,15,0,0,"sub-sequence", 1, 2, 0);
	trans[6][15]	= settr(441,2,19,46,46,"Sensor_reading_x_tmp = 4", 1, 2, 0);
	trans[6][20]	= settr(446,2,19,1,0,".(goto)", 1, 2, 0);
	T = trans[6][19] = settr(445,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(445,2,18,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(445,2,16,0,0,"DO", 1, 2, 0);
	trans[6][18]	= settr(444,2,36,47,47,"goto :b19", 1, 2, 0); /* m: 33 -> 0,36 */
	reached6[33] = 1;
	trans[6][16]	= settr(442,2,19,48,48,"((Sensor_reading_x_tmp<4))", 1, 2, 0); /* m: 17 -> 19,0 */
	reached6[17] = 1;
	trans[6][17]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[6][21]	= settr(447,2,33,1,0,"break", 1, 2, 0);
	T = trans[ 6][30] = settr(456,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(456,0,23,0,0,"sub-sequence", 1, 2, 0);
	trans[6][23]	= settr(449,2,27,49,49,"Sensor_reading_x_tmp = 10", 1, 2, 0);
	trans[6][28]	= settr(454,2,27,1,0,".(goto)", 1, 2, 0);
	T = trans[6][27] = settr(453,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(453,2,26,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(453,2,24,0,0,"DO", 1, 2, 0);
	trans[6][26]	= settr(452,2,36,50,50,"goto :b20", 1, 2, 0); /* m: 33 -> 0,36 */
	reached6[33] = 1;
	trans[6][24]	= settr(450,2,27,51,51,"((Sensor_reading_x_tmp<10))", 1, 2, 0); /* m: 25 -> 27,0 */
	reached6[25] = 1;
	trans[6][25]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[6][29]	= settr(455,2,33,1,0,"break", 1, 2, 0);
	trans[6][33]	= settr(459,2,36,52,52,"value = Sensor_reading_x_tmp", 1, 2, 0);
	T = trans[ 6][36] = settr(462,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(462,0,35,0,0,"sub-sequence", 1, 2, 0);
	trans[6][35]	= settr(461,0,41,53,53,"Modemanager_reading_channel!value", 1, 6, 0); /* m: 38 -> 41,0 */
	reached6[38] = 1;
	trans[6][38]	= settr(0,0,0,0,0,"inputVectorCounter = (inputVectorCounter+1)",0,0,0);
	trans[6][39]	= settr(465,0,44,2,0,"else", 0, 2, 0);
	trans[6][40]	= settr(466,0,44,1,0,"goto :b17", 0, 2, 0);
	trans[6][43]	= settr(469,0,44,1,0,"break", 0, 2, 0);
	trans[6][44]	= settr(470,0,0,54,54,"-end-", 0, 3500, 0);

	/* proctype 5: Modemanager_tc */

	trans[5] = (Trans **) emalloc(85*sizeof(Trans *));

	trans[5][1]	= settr(343,0,81,55,0,"(inited)", 1, 2, 0);
	trans[5][82]	= settr(424,0,81,1,0,".(goto)", 0, 2, 0);
	T = trans[5][81] = settr(423,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(423,0,80,0,0,"DO", 0, 2, 0);
	T = trans[ 5][80] = settr(422,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(422,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[5][2]	= settr(344,2,3,56,0,"(nempty(Modemanager_tc_channel))", 1, 2500, 0);
	trans[5][3]	= settr(345,2,77,57,57,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[5][77] = settr(419,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(419,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(419,2,75,0,0,"IF", 1, 2500, 0);
	trans[5][4]	= settr(346,2,5,58,0,"(nempty(Modemanager_tc_channel))", 1, 2500, 0);
	trans[5][5]	= settr(347,2,73,59,59,"Modemanager_tc_channel?Modemanager_tc_signal_parameter.param_a,Modemanager_tc_signal_parameter.param_b", 1, 2500, 0); /* m: 6 -> 73,0 */
	reached5[6] = 1;
	trans[5][6]	= settr(0,0,0,0,0,"Modemanager_tc_channel_used = 1",0,0,0);
	T = trans[ 5][73] = settr(415,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(415,0,71,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[5][71] = settr(413,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(413,2,7,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(413,2,69,0,0,"IF", 1, 2500, 0);
	trans[5][7]	= settr(349,2,16,60,0,"((global_state.modemanager.state==1))", 1, 2500, 0);
	T = trans[ 5][16] = settr(358,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(358,0,11,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][11] = settr(353,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(353,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][8]	= settr(350,2,10,61,61,"global_state.modemanager.cfg.param_a = Modemanager_tc_signal_parameter.param_a", 1, 2500, 0);
	T = trans[ 5][10] = settr(352,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(352,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][9]	= settr(351,2,15,62,0,"assert(((global_state.modemanager.cfg.param_a>=0)&&(global_state.modemanager.cfg.param_a<=1000000)))", 1, 2500, 0);
	T = trans[ 5][15] = settr(357,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(357,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][12]	= settr(354,2,14,63,63,"global_state.modemanager.cfg.param_b = Modemanager_tc_signal_parameter.param_b", 1, 2500, 0);
	T = trans[ 5][14] = settr(356,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(356,0,13,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][13]	= settr(355,2,68,64,0,"assert(((global_state.modemanager.cfg.param_b>=0)&&(global_state.modemanager.cfg.param_b<=1000000)))", 1, 2500, 0);
	T = trans[ 5][68] = settr(410,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(410,0,17,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][17]	= settr(359,2,65,65,65,"transition_id = 0", 1, 2500, 0); /* m: 18 -> 0,65 */
	reached5[18] = 1;
	trans[5][18]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[5][66]	= settr(408,2,65,1,0,".(goto)", 1, 2500, 0);
	T = trans[5][65] = settr(407,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(407,2,62,0,0,"DO", 1, 2500, 0);
	T = trans[5][62] = settr(404,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(404,2,19,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(404,2,21,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(404,2,25,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(404,2,45,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(404,2,56,0,0,"IF", 1, 2500, 0);
	trans[5][19]	= settr(361,2,77,66,66,"((transition_id==-(1)))", 1, 2500, 0);
	trans[5][20]	= settr(362,2,77,1,0,"goto :b16", 1, 2500, 0);
	trans[5][63]	= settr(405,2,64,1,0,".(goto)", 1, 2500, 0);
	trans[5][21]	= settr(363,2,64,67,67,"((transition_id==0))", 1, 2500, 0); /* m: 22 -> 64,0 */
	reached5[22] = 1;
	trans[5][22]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][23]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[5][24]	= settr(366,2,64,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][25]	= settr(367,2,27,68,68,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 5][27] = settr(369,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(369,0,26,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][26]	= settr(368,2,40,1,0,"(1)", 1, 2500, 0);
	T = trans[ 5][40] = settr(382,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(382,0,31,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][31] = settr(373,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(373,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][28]	= settr(370,2,30,69,69,"global_state.modemanager.rprt.reading = global_state.modemanager.xin", 1, 2500, 0);
	T = trans[ 5][30] = settr(372,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(372,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][29]	= settr(371,2,35,70,0,"assert(((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=1000000)))", 1, 2500, 0);
	T = trans[ 5][35] = settr(377,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(377,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][32]	= settr(374,2,34,71,71,"global_state.modemanager.rprt.ctrl = global_state.modemanager.xout", 1, 2500, 0);
	T = trans[ 5][34] = settr(376,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(376,0,33,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][33]	= settr(375,2,39,72,0,"assert(((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=1000000)))", 1, 2500, 0);
	T = trans[ 5][39] = settr(381,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(381,0,36,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][36]	= settr(378,2,38,73,73,"global_state.modemanager.rprt.feedback = global_state.modemanager.xfeedback", 1, 2500, 0);
	T = trans[ 5][38] = settr(380,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(380,0,37,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][37]	= settr(379,2,42,74,0,"assert(((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=1000000)))", 1, 2500, 0);
	T = trans[ 5][42] = settr(384,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(384,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][41]	= settr(383,2,64,75,75,"Egse_tm_channel!global_state.modemanager.rprt.reading,global_state.modemanager.rprt.ctrl,global_state.modemanager.rprt.feedback", 1, 2500, 0); /* m: 43 -> 64,0 */
	reached5[43] = 1;
	trans[5][43]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][44]	= settr(386,2,64,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][45]	= settr(387,2,47,76,76,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 5][47] = settr(389,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(389,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][46]	= settr(388,2,51,1,0,"(1)", 1, 2500, 0);
	T = trans[ 5][51] = settr(393,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(393,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][48]	= settr(390,2,50,77,77,"global_state.modemanager.xout = ((global_state.modemanager.xin*global_state.modemanager.cfg.param_a)+global_state.modemanager.cfg.param_b)", 1, 2500, 0);
	T = trans[ 5][50] = settr(392,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(392,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][49]	= settr(391,2,53,78,0,"assert(((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=1000000)))", 1, 2500, 0);
	T = trans[ 5][53] = settr(395,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(395,0,52,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][52]	= settr(394,2,64,79,79,"Actuator_ctrl_channel!global_state.modemanager.xout", 1, 2500, 0); /* m: 54 -> 64,0 */
	reached5[54] = 1;
	trans[5][54]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][55]	= settr(397,2,64,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][56]	= settr(398,2,58,80,80,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 5][58] = settr(400,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(400,0,57,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][57]	= settr(399,2,64,81,81,"(1)", 1, 2500, 0); /* m: 59 -> 64,0 */
	reached5[59] = 1;
	trans[5][59]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][60]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[5][61]	= settr(403,2,64,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][64]	= settr(406,2,65,1,0,"(1)", 1, 2500, 0);
	trans[5][67]	= settr(409,2,77,1,0,"break", 1, 2500, 0);
	trans[5][72]	= settr(414,2,77,1,0,".(goto)", 1, 2500, 0);
	trans[5][69]	= settr(411,2,70,2,0,"else", 1, 2500, 0);
	trans[5][70]	= settr(412,2,77,1,0,"(1)", 1, 2500, 0);
	trans[5][74]	= settr(416,2,77,1,0,"goto Modemanager_tc_loop", 1, 2500, 0);
	trans[5][78]	= settr(420,2,79,1,0,".(goto)", 1, 2500, 0);
	trans[5][75]	= settr(417,2,76,82,0,"(empty(Modemanager_tc_channel))", 1, 2500, 0);
	trans[5][76]	= settr(418,2,79,83,83,"(1)", 1, 2500, 0); /* m: 78 -> 79,0 */
	reached5[78] = 1;
	trans[5][79]	= settr(421,0,81,84,84,"Modemanager_lock!1", 1, 2500, 0);
	trans[5][83]	= settr(425,0,84,1,0,"break", 0, 2, 0);
	trans[5][84]	= settr(426,0,0,85,85,"-end-", 0, 3500, 0);

	/* proctype 4: Modemanager_reading */

	trans[4] = (Trans **) emalloc(80*sizeof(Trans *));

	trans[4][1]	= settr(264,0,76,86,0,"(inited)", 1, 2, 0);
	trans[4][77]	= settr(340,0,76,1,0,".(goto)", 0, 2, 0);
	T = trans[4][76] = settr(339,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(339,0,75,0,0,"DO", 0, 2, 0);
	T = trans[ 4][75] = settr(338,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(338,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[4][2]	= settr(265,2,3,87,0,"(nempty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[4][3]	= settr(266,2,72,88,88,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[4][72] = settr(335,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(335,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(335,2,70,0,0,"IF", 1, 2500, 0);
	trans[4][4]	= settr(267,2,5,89,0,"(nempty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[4][5]	= settr(268,2,68,90,90,"Modemanager_reading_channel?Modemanager_reading_signal_parameter", 1, 2500, 0); /* m: 6 -> 68,0 */
	reached4[6] = 1;
	trans[4][6]	= settr(0,0,0,0,0,"Modemanager_reading_channel_used = 1",0,0,0);
	T = trans[ 4][68] = settr(331,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(331,0,66,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[4][66] = settr(329,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(329,2,7,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(329,2,64,0,0,"IF", 1, 2500, 0);
	trans[4][7]	= settr(270,2,11,91,0,"((global_state.modemanager.state==0))", 1, 2500, 0);
	T = trans[ 4][11] = settr(274,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(274,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][8]	= settr(271,2,10,92,92,"global_state.modemanager.xin = Modemanager_reading_signal_parameter", 1, 2500, 0);
	T = trans[ 4][10] = settr(273,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(273,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][9]	= settr(272,2,63,93,0,"assert(((global_state.modemanager.xin>=0)&&(global_state.modemanager.xin<=1000000)))", 1, 2500, 0);
	T = trans[ 4][63] = settr(326,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(326,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][12]	= settr(275,2,60,94,94,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,60 */
	reached4[13] = 1;
	trans[4][13]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[4][61]	= settr(324,2,60,1,0,".(goto)", 1, 2500, 0);
	T = trans[4][60] = settr(323,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(323,2,57,0,0,"DO", 1, 2500, 0);
	T = trans[4][57] = settr(320,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(320,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(320,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(320,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(320,2,40,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(320,2,51,0,0,"IF", 1, 2500, 0);
	trans[4][14]	= settr(277,2,72,95,95,"((transition_id==-(1)))", 1, 2500, 0);
	trans[4][15]	= settr(278,2,72,1,0,"goto :b14", 1, 2500, 0);
	trans[4][58]	= settr(321,2,59,1,0,".(goto)", 1, 2500, 0);
	trans[4][16]	= settr(279,2,59,96,96,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 59,0 */
	reached4[17] = 1;
	trans[4][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[4][19]	= settr(282,2,59,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][20]	= settr(283,2,22,97,97,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 4][22] = settr(285,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(285,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][21]	= settr(284,2,35,1,0,"(1)", 1, 2500, 0);
	T = trans[ 4][35] = settr(298,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(298,0,26,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][26] = settr(289,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(289,0,23,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][23]	= settr(286,2,25,98,98,"global_state.modemanager.rprt.reading = global_state.modemanager.xin", 1, 2500, 0);
	T = trans[ 4][25] = settr(288,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(288,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][24]	= settr(287,2,30,99,0,"assert(((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=1000000)))", 1, 2500, 0);
	T = trans[ 4][30] = settr(293,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(293,0,27,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][27]	= settr(290,2,29,100,100,"global_state.modemanager.rprt.ctrl = global_state.modemanager.xout", 1, 2500, 0);
	T = trans[ 4][29] = settr(292,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(292,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][28]	= settr(291,2,34,101,0,"assert(((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=1000000)))", 1, 2500, 0);
	T = trans[ 4][34] = settr(297,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(297,0,31,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][31]	= settr(294,2,33,102,102,"global_state.modemanager.rprt.feedback = global_state.modemanager.xfeedback", 1, 2500, 0);
	T = trans[ 4][33] = settr(296,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(296,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][32]	= settr(295,2,37,103,0,"assert(((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=1000000)))", 1, 2500, 0);
	T = trans[ 4][37] = settr(300,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(300,0,36,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][36]	= settr(299,2,59,104,104,"Egse_tm_channel!global_state.modemanager.rprt.reading,global_state.modemanager.rprt.ctrl,global_state.modemanager.rprt.feedback", 1, 2500, 0); /* m: 38 -> 59,0 */
	reached4[38] = 1;
	trans[4][38]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][39]	= settr(302,2,59,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][40]	= settr(303,2,42,105,105,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 4][42] = settr(305,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(305,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][41]	= settr(304,2,46,1,0,"(1)", 1, 2500, 0);
	T = trans[ 4][46] = settr(309,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(309,0,43,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][43]	= settr(306,2,45,106,106,"global_state.modemanager.xout = ((global_state.modemanager.xin*global_state.modemanager.cfg.param_a)+global_state.modemanager.cfg.param_b)", 1, 2500, 0);
	T = trans[ 4][45] = settr(308,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(308,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][44]	= settr(307,2,48,107,0,"assert(((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=1000000)))", 1, 2500, 0);
	T = trans[ 4][48] = settr(311,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(311,0,47,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][47]	= settr(310,2,59,108,108,"Actuator_ctrl_channel!global_state.modemanager.xout", 1, 2500, 0); /* m: 49 -> 59,0 */
	reached4[49] = 1;
	trans[4][49]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][50]	= settr(313,2,59,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][51]	= settr(314,2,53,109,109,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 4][53] = settr(316,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(316,0,52,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][52]	= settr(315,2,59,110,110,"(1)", 1, 2500, 0); /* m: 54 -> 59,0 */
	reached4[54] = 1;
	trans[4][54]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][55]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[4][56]	= settr(319,2,59,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][59]	= settr(322,2,60,1,0,"(1)", 1, 2500, 0);
	trans[4][62]	= settr(325,2,72,1,0,"break", 1, 2500, 0);
	trans[4][67]	= settr(330,2,72,1,0,".(goto)", 1, 2500, 0);
	trans[4][64]	= settr(327,2,65,2,0,"else", 1, 2500, 0);
	trans[4][65]	= settr(328,2,72,1,0,"(1)", 1, 2500, 0);
	trans[4][69]	= settr(332,2,72,1,0,"goto Modemanager_reading_loop", 1, 2500, 0);
	trans[4][73]	= settr(336,2,74,1,0,".(goto)", 1, 2500, 0);
	trans[4][70]	= settr(333,2,71,111,0,"(empty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[4][71]	= settr(334,2,74,112,112,"(1)", 1, 2500, 0); /* m: 73 -> 74,0 */
	reached4[73] = 1;
	trans[4][74]	= settr(337,0,76,113,113,"Modemanager_lock!1", 1, 2500, 0);
	trans[4][78]	= settr(341,0,79,1,0,"break", 0, 2, 0);
	trans[4][79]	= settr(342,0,0,114,114,"-end-", 0, 3500, 0);

	/* proctype 3: Modemanager_feedback */

	trans[3] = (Trans **) emalloc(80*sizeof(Trans *));

	trans[3][1]	= settr(185,0,76,115,0,"(inited)", 1, 2, 0);
	trans[3][77]	= settr(261,0,76,1,0,".(goto)", 0, 2, 0);
	T = trans[3][76] = settr(260,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(260,0,75,0,0,"DO", 0, 2, 0);
	T = trans[ 3][75] = settr(259,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(259,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[3][2]	= settr(186,2,3,116,0,"(nempty(Modemanager_feedback_channel))", 1, 2500, 0);
	trans[3][3]	= settr(187,2,72,117,117,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[3][72] = settr(256,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(256,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(256,2,70,0,0,"IF", 1, 2500, 0);
	trans[3][4]	= settr(188,2,5,118,0,"(nempty(Modemanager_feedback_channel))", 1, 2500, 0);
	trans[3][5]	= settr(189,2,68,119,119,"Modemanager_feedback_channel?Modemanager_feedback_signal_parameter", 1, 2500, 0); /* m: 6 -> 68,0 */
	reached3[6] = 1;
	trans[3][6]	= settr(0,0,0,0,0,"Modemanager_feedback_channel_used = 1",0,0,0);
	T = trans[ 3][68] = settr(252,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(252,0,66,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[3][66] = settr(250,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(250,2,7,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(250,2,64,0,0,"IF", 1, 2500, 0);
	trans[3][7]	= settr(191,2,11,120,0,"((global_state.modemanager.state==0))", 1, 2500, 0);
	T = trans[ 3][11] = settr(195,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(195,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][8]	= settr(192,2,10,121,121,"global_state.modemanager.xfeedback = Modemanager_feedback_signal_parameter", 1, 2500, 0);
	T = trans[ 3][10] = settr(194,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(194,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][9]	= settr(193,2,63,122,0,"assert(((global_state.modemanager.xfeedback>=0)&&(global_state.modemanager.xfeedback<=1000000)))", 1, 2500, 0);
	T = trans[ 3][63] = settr(247,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(247,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][12]	= settr(196,2,60,123,123,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,60 */
	reached3[13] = 1;
	trans[3][13]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[3][61]	= settr(245,2,60,1,0,".(goto)", 1, 2500, 0);
	T = trans[3][60] = settr(244,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(244,2,57,0,0,"DO", 1, 2500, 0);
	T = trans[3][57] = settr(241,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(241,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(241,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(241,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(241,2,40,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(241,2,51,0,0,"IF", 1, 2500, 0);
	trans[3][14]	= settr(198,2,72,124,124,"((transition_id==-(1)))", 1, 2500, 0);
	trans[3][15]	= settr(199,2,72,1,0,"goto :b12", 1, 2500, 0);
	trans[3][58]	= settr(242,2,59,1,0,".(goto)", 1, 2500, 0);
	trans[3][16]	= settr(200,2,59,125,125,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 59,0 */
	reached3[17] = 1;
	trans[3][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[3][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[3][19]	= settr(203,2,59,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[3][20]	= settr(204,2,22,126,126,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 3][22] = settr(206,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(206,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][21]	= settr(205,2,35,1,0,"(1)", 1, 2500, 0);
	T = trans[ 3][35] = settr(219,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(219,0,26,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 3][26] = settr(210,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(210,0,23,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][23]	= settr(207,2,25,127,127,"global_state.modemanager.rprt.reading = global_state.modemanager.xin", 1, 2500, 0);
	T = trans[ 3][25] = settr(209,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(209,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][24]	= settr(208,2,30,128,0,"assert(((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=1000000)))", 1, 2500, 0);
	T = trans[ 3][30] = settr(214,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(214,0,27,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][27]	= settr(211,2,29,129,129,"global_state.modemanager.rprt.ctrl = global_state.modemanager.xout", 1, 2500, 0);
	T = trans[ 3][29] = settr(213,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(213,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][28]	= settr(212,2,34,130,0,"assert(((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=1000000)))", 1, 2500, 0);
	T = trans[ 3][34] = settr(218,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(218,0,31,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][31]	= settr(215,2,33,131,131,"global_state.modemanager.rprt.feedback = global_state.modemanager.xfeedback", 1, 2500, 0);
	T = trans[ 3][33] = settr(217,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(217,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][32]	= settr(216,2,37,132,0,"assert(((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=1000000)))", 1, 2500, 0);
	T = trans[ 3][37] = settr(221,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(221,0,36,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][36]	= settr(220,2,59,133,133,"Egse_tm_channel!global_state.modemanager.rprt.reading,global_state.modemanager.rprt.ctrl,global_state.modemanager.rprt.feedback", 1, 2500, 0); /* m: 38 -> 59,0 */
	reached3[38] = 1;
	trans[3][38]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[3][39]	= settr(223,2,59,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[3][40]	= settr(224,2,42,134,134,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 3][42] = settr(226,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(226,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][41]	= settr(225,2,46,1,0,"(1)", 1, 2500, 0);
	T = trans[ 3][46] = settr(230,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(230,0,43,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][43]	= settr(227,2,45,135,135,"global_state.modemanager.xout = ((global_state.modemanager.xin*global_state.modemanager.cfg.param_a)+global_state.modemanager.cfg.param_b)", 1, 2500, 0);
	T = trans[ 3][45] = settr(229,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(229,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][44]	= settr(228,2,48,136,0,"assert(((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=1000000)))", 1, 2500, 0);
	T = trans[ 3][48] = settr(232,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(232,0,47,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][47]	= settr(231,2,59,137,137,"Actuator_ctrl_channel!global_state.modemanager.xout", 1, 2500, 0); /* m: 49 -> 59,0 */
	reached3[49] = 1;
	trans[3][49]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[3][50]	= settr(234,2,59,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[3][51]	= settr(235,2,53,138,138,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 3][53] = settr(237,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(237,0,52,0,0,"sub-sequence", 1, 2500, 0);
	trans[3][52]	= settr(236,2,59,139,139,"(1)", 1, 2500, 0); /* m: 54 -> 59,0 */
	reached3[54] = 1;
	trans[3][54]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[3][55]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[3][56]	= settr(240,2,59,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[3][59]	= settr(243,2,60,1,0,"(1)", 1, 2500, 0);
	trans[3][62]	= settr(246,2,72,1,0,"break", 1, 2500, 0);
	trans[3][67]	= settr(251,2,72,1,0,".(goto)", 1, 2500, 0);
	trans[3][64]	= settr(248,2,65,2,0,"else", 1, 2500, 0);
	trans[3][65]	= settr(249,2,72,1,0,"(1)", 1, 2500, 0);
	trans[3][69]	= settr(253,2,72,1,0,"goto Modemanager_feedback_loop", 1, 2500, 0);
	trans[3][73]	= settr(257,2,74,1,0,".(goto)", 1, 2500, 0);
	trans[3][70]	= settr(254,2,71,140,0,"(empty(Modemanager_feedback_channel))", 1, 2500, 0);
	trans[3][71]	= settr(255,2,74,141,141,"(1)", 1, 2500, 0); /* m: 73 -> 74,0 */
	reached3[73] = 1;
	trans[3][74]	= settr(258,0,76,142,142,"Modemanager_lock!1", 1, 2500, 0);
	trans[3][78]	= settr(262,0,79,1,0,"break", 0, 2, 0);
	trans[3][79]	= settr(263,0,0,143,143,"-end-", 0, 3500, 0);

	/* proctype 2: Egse_tc */

	trans[2] = (Trans **) emalloc(75*sizeof(Trans *));

	trans[2][1]	= settr(111,0,2,144,0,"(inited)", 1, 2, 0);
	trans[2][2]	= settr(112,0,71,145,145,"", 0, 2, 0); /* m: 3 -> 0,71 */
	reached2[3] = 1;
	trans[2][3]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[2][4]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[2][72]	= settr(182,0,71,1,0,".(goto)", 0, 2, 0);
	T = trans[2][71] = settr(181,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(181,0,5,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(181,0,69,0,0,"DO", 0, 2, 0);
	trans[2][5]	= settr(115,0,67,146,0,"((inputVectorCounter<=3))", 0, 2, 0);
	T = trans[ 2][67] = settr(177,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(177,2,64,0,0,"ATOMIC", 1, 2, 0);
	T = trans[ 2][64] = settr(174,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(174,0,34,0,0,"sub-sequence", 1, 2, 0);
	T = trans[ 2][34] = settr(144,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(144,0,6,0,0,"sub-sequence", 1, 2, 0);
	trans[2][6]	= settr(116,2,31,147,147,"T_SmallInteger_tmp = 0", 1, 2, 0);
	T = trans[2][31] = settr(141,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(141,2,14,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(141,2,22,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(141,2,30,0,0,"IF", 1, 2, 0);
	T = trans[ 2][14] = settr(124,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(124,0,7,0,0,"sub-sequence", 1, 2, 0);
	trans[2][7]	= settr(117,2,11,148,148,"T_SmallInteger_tmp = 0", 1, 2, 0);
	trans[2][12]	= settr(122,2,11,1,0,".(goto)", 1, 2, 0);
	T = trans[2][11] = settr(121,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(121,2,10,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(121,2,8,0,0,"DO", 1, 2, 0);
	trans[2][10]	= settr(120,2,63,149,149,"goto :b5", 1, 2, 0); /* m: 33 -> 0,63 */
	reached2[33] = 1;
	trans[2][8]	= settr(118,2,11,150,150,"((T_SmallInteger_tmp<0))", 1, 2, 0); /* m: 9 -> 11,0 */
	reached2[9] = 1;
	trans[2][9]	= settr(0,0,0,0,0,"T_SmallInteger_tmp = (T_SmallInteger_tmp+1)",0,0,0);
	trans[2][13]	= settr(123,2,33,1,0,"break", 1, 2, 0);
	trans[2][32]	= settr(142,2,33,1,0,".(goto)", 1, 2, 0); /* m: 33 -> 0,63 */
	reached2[33] = 1;
	T = trans[ 2][22] = settr(132,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(132,0,15,0,0,"sub-sequence", 1, 2, 0);
	trans[2][15]	= settr(125,2,19,151,151,"T_SmallInteger_tmp = 4", 1, 2, 0);
	trans[2][20]	= settr(130,2,19,1,0,".(goto)", 1, 2, 0);
	T = trans[2][19] = settr(129,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(129,2,18,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(129,2,16,0,0,"DO", 1, 2, 0);
	trans[2][18]	= settr(128,2,63,152,152,"goto :b6", 1, 2, 0); /* m: 33 -> 0,63 */
	reached2[33] = 1;
	trans[2][16]	= settr(126,2,19,153,153,"((T_SmallInteger_tmp<4))", 1, 2, 0); /* m: 17 -> 19,0 */
	reached2[17] = 1;
	trans[2][17]	= settr(0,0,0,0,0,"T_SmallInteger_tmp = (T_SmallInteger_tmp+1)",0,0,0);
	trans[2][21]	= settr(131,2,33,1,0,"break", 1, 2, 0);
	T = trans[ 2][30] = settr(140,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(140,0,23,0,0,"sub-sequence", 1, 2, 0);
	trans[2][23]	= settr(133,2,27,154,154,"T_SmallInteger_tmp = 10", 1, 2, 0);
	trans[2][28]	= settr(138,2,27,1,0,".(goto)", 1, 2, 0);
	T = trans[2][27] = settr(137,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(137,2,26,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(137,2,24,0,0,"DO", 1, 2, 0);
	trans[2][26]	= settr(136,2,63,155,155,"goto :b7", 1, 2, 0); /* m: 33 -> 0,63 */
	reached2[33] = 1;
	trans[2][24]	= settr(134,2,27,156,156,"((T_SmallInteger_tmp<10))", 1, 2, 0); /* m: 25 -> 27,0 */
	reached2[25] = 1;
	trans[2][25]	= settr(0,0,0,0,0,"T_SmallInteger_tmp = (T_SmallInteger_tmp+1)",0,0,0);
	trans[2][29]	= settr(139,2,33,1,0,"break", 1, 2, 0);
	trans[2][33]	= settr(143,2,63,157,157,"value.param_a = T_SmallInteger_tmp", 1, 2, 0);
	T = trans[ 2][63] = settr(173,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(173,0,35,0,0,"sub-sequence", 1, 2, 0);
	trans[2][35]	= settr(145,2,60,158,158,"T_SmallInteger_tmp = 0", 1, 2, 0);
	T = trans[2][60] = settr(170,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(170,2,43,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(170,2,51,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(170,2,59,0,0,"IF", 1, 2, 0);
	T = trans[ 2][43] = settr(153,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(153,0,36,0,0,"sub-sequence", 1, 2, 0);
	trans[2][36]	= settr(146,2,40,159,159,"T_SmallInteger_tmp = 0", 1, 2, 0);
	trans[2][41]	= settr(151,2,40,1,0,".(goto)", 1, 2, 0);
	T = trans[2][40] = settr(150,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(150,2,39,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(150,2,37,0,0,"DO", 1, 2, 0);
	trans[2][39]	= settr(149,2,66,160,160,"goto :b8", 1, 2, 0); /* m: 62 -> 0,66 */
	reached2[62] = 1;
	trans[2][37]	= settr(147,2,40,161,161,"((T_SmallInteger_tmp<0))", 1, 2, 0); /* m: 38 -> 40,0 */
	reached2[38] = 1;
	trans[2][38]	= settr(0,0,0,0,0,"T_SmallInteger_tmp = (T_SmallInteger_tmp+1)",0,0,0);
	trans[2][42]	= settr(152,2,62,1,0,"break", 1, 2, 0);
	trans[2][61]	= settr(171,2,62,1,0,".(goto)", 1, 2, 0); /* m: 62 -> 0,66 */
	reached2[62] = 1;
	T = trans[ 2][51] = settr(161,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(161,0,44,0,0,"sub-sequence", 1, 2, 0);
	trans[2][44]	= settr(154,2,48,162,162,"T_SmallInteger_tmp = 4", 1, 2, 0);
	trans[2][49]	= settr(159,2,48,1,0,".(goto)", 1, 2, 0);
	T = trans[2][48] = settr(158,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(158,2,47,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(158,2,45,0,0,"DO", 1, 2, 0);
	trans[2][47]	= settr(157,2,66,163,163,"goto :b9", 1, 2, 0); /* m: 62 -> 0,66 */
	reached2[62] = 1;
	trans[2][45]	= settr(155,2,48,164,164,"((T_SmallInteger_tmp<4))", 1, 2, 0); /* m: 46 -> 48,0 */
	reached2[46] = 1;
	trans[2][46]	= settr(0,0,0,0,0,"T_SmallInteger_tmp = (T_SmallInteger_tmp+1)",0,0,0);
	trans[2][50]	= settr(160,2,62,1,0,"break", 1, 2, 0);
	T = trans[ 2][59] = settr(169,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(169,0,52,0,0,"sub-sequence", 1, 2, 0);
	trans[2][52]	= settr(162,2,56,165,165,"T_SmallInteger_tmp = 10", 1, 2, 0);
	trans[2][57]	= settr(167,2,56,1,0,".(goto)", 1, 2, 0);
	T = trans[2][56] = settr(166,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(166,2,55,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(166,2,53,0,0,"DO", 1, 2, 0);
	trans[2][55]	= settr(165,2,66,166,166,"goto :b10", 1, 2, 0); /* m: 62 -> 0,66 */
	reached2[62] = 1;
	trans[2][53]	= settr(163,2,56,167,167,"((T_SmallInteger_tmp<10))", 1, 2, 0); /* m: 54 -> 56,0 */
	reached2[54] = 1;
	trans[2][54]	= settr(0,0,0,0,0,"T_SmallInteger_tmp = (T_SmallInteger_tmp+1)",0,0,0);
	trans[2][58]	= settr(168,2,62,1,0,"break", 1, 2, 0);
	trans[2][62]	= settr(172,2,66,168,168,"value.param_b = T_SmallInteger_tmp", 1, 2, 0);
	T = trans[ 2][66] = settr(176,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(176,0,65,0,0,"sub-sequence", 1, 2, 0);
	trans[2][65]	= settr(175,0,71,169,169,"Modemanager_tc_channel!value.param_a,value.param_b", 1, 7, 0); /* m: 68 -> 71,0 */
	reached2[68] = 1;
	trans[2][68]	= settr(0,0,0,0,0,"inputVectorCounter = (inputVectorCounter+1)",0,0,0);
	trans[2][69]	= settr(179,0,74,2,0,"else", 0, 2, 0);
	trans[2][70]	= settr(180,0,74,1,0,"goto :b4", 0, 2, 0);
	trans[2][73]	= settr(183,0,74,1,0,"break", 0, 2, 0);
	trans[2][74]	= settr(184,0,0,170,170,"-end-", 0, 3500, 0);

	/* proctype 1: Egse_tm */

	trans[1] = (Trans **) emalloc(16*sizeof(Trans *));

	trans[1][1]	= settr(96,0,12,171,0,"(inited)", 1, 2, 0);
	trans[1][13]	= settr(108,0,12,1,0,".(goto)", 0, 2, 0);
	T = trans[1][12] = settr(107,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(107,0,11,0,0,"DO", 0, 2, 0);
	T = trans[ 1][11] = settr(106,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(106,2,2,0,0,"ATOMIC", 1, 504, 1004);
	trans[1][2]	= settr(97,2,9,172,0,"(nempty(Egse_tm_channel))", 1, 504, 1004);
	T = trans[1][9] = settr(104,2,0,0,0,"IF", 1, 504, 1004);
	T = T->nxt	= settr(104,2,3,0,0,"IF", 1, 504, 1004);
	    T->nxt	= settr(104,2,7,0,0,"IF", 1, 504, 1004);
	trans[1][3]	= settr(98,2,4,173,0,"(nempty(Egse_tm_channel))", 1, 504, 1004);
	trans[1][4]	= settr(99,2,9,174,174,"Egse_tm_channel?Egse_tm_signal_parameter.reading,Egse_tm_signal_parameter.ctrl,Egse_tm_signal_parameter.feedback", 1, 504, 1004); /* m: 5 -> 9,0 */
	reached1[5] = 1;
	trans[1][5]	= settr(0,0,0,0,0,"Egse_tm_channel_used = 1",0,0,0);
	trans[1][6]	= settr(101,2,9,1,0,"goto Egse_tm_loop", 1, 504, 1004);
	trans[1][10]	= settr(105,0,12,1,0,".(goto)", 1, 504, 1004);
	trans[1][7]	= settr(102,2,8,175,0,"(empty(Egse_tm_channel))", 1, 504, 1004);
	trans[1][8]	= settr(103,0,12,176,176,"(1)", 1, 504, 1004); /* m: 10 -> 12,0 */
	reached1[10] = 1;
	trans[1][14]	= settr(109,0,15,1,0,"break", 0, 2, 0);
	trans[1][15]	= settr(110,0,0,177,177,"-end-", 0, 3500, 0);

	/* proctype 0: Actuator_ctrl */

	trans[0] = (Trans **) emalloc(97*sizeof(Trans *));

	trans[0][1]	= settr(0,0,93,178,0,"(inited)", 1, 2, 0);
	trans[0][94]	= settr(93,0,93,1,0,".(goto)", 0, 2, 0);
	T = trans[0][93] = settr(92,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(92,0,92,0,0,"DO", 0, 2, 0);
	T = trans[ 0][92] = settr(91,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(91,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[0][2]	= settr(1,2,3,179,0,"(nempty(Actuator_ctrl_channel))", 1, 2500, 0);
	trans[0][3]	= settr(2,2,89,180,180,"Actuator_lock?_", 1, 2500, 0);
	T = trans[0][89] = settr(88,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(88,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(88,2,87,0,0,"IF", 1, 2500, 0);
	trans[0][4]	= settr(3,2,5,181,0,"(nempty(Actuator_ctrl_channel))", 1, 2500, 0);
	trans[0][5]	= settr(4,2,85,182,182,"Actuator_ctrl_channel?Actuator_ctrl_signal_parameter", 1, 2500, 0); /* m: 6 -> 85,0 */
	reached0[6] = 1;
	trans[0][6]	= settr(0,0,0,0,0,"Actuator_ctrl_channel_used = 1",0,0,0);
	T = trans[ 0][85] = settr(84,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(84,0,83,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[0][83] = settr(82,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(82,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(82,2,44,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(82,2,81,0,0,"IF", 1, 2500, 0);
	trans[0][7]	= settr(6,2,11,183,0,"((global_state.actuator.state==0))", 1, 2500, 0);
	T = trans[ 0][11] = settr(10,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(10,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][8]	= settr(7,2,10,184,184,"global_state.actuator.x = Actuator_ctrl_signal_parameter", 1, 2500, 0);
	T = trans[ 0][10] = settr(9,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(9,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][9]	= settr(8,2,43,185,0,"assert(((global_state.actuator.x>=0)&&(global_state.actuator.x<=1000000)))", 1, 2500, 0);
	T = trans[ 0][43] = settr(42,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(42,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][12]	= settr(11,2,40,186,186,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,40 */
	reached0[13] = 1;
	trans[0][13]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[0][41]	= settr(40,2,40,1,0,".(goto)", 1, 2500, 0);
	T = trans[0][40] = settr(39,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(39,2,37,0,0,"DO", 1, 2500, 0);
	T = trans[0][37] = settr(36,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(36,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(36,2,16,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(36,2,20,0,0,"IF", 1, 2500, 0);
	trans[0][14]	= settr(13,2,89,187,187,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][15]	= settr(14,2,89,1,0,"goto :b1", 1, 2500, 0);
	trans[0][38]	= settr(37,2,39,1,0,".(goto)", 1, 2500, 0);
	trans[0][16]	= settr(15,2,39,188,188,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 39,0 */
	reached0[17] = 1;
	trans[0][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][18]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[0][19]	= settr(18,2,39,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][20]	= settr(19,2,22,189,189,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][22] = settr(21,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(21,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][21]	= settr(20,2,35,1,0,"(1)", 1, 2500, 0);
	T = trans[0][35] = settr(34,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(34,2,23,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(34,2,29,0,0,"IF", 1, 2500, 0);
	trans[0][23]	= settr(22,2,25,190,0,"(((global_state.actuator.x>10)==1))", 1, 2500, 0);
	T = trans[ 0][25] = settr(24,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(24,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][24]	= settr(23,2,39,191,191,"Modemanager_feedback_channel!global_state.actuator.x", 1, 2500, 0); /* m: 26 -> 39,0 */
	reached0[26] = 1;
	trans[0][26]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][27]	= settr(0,0,0,0,0,"global_state.actuator.state = 1",0,0,0);
	trans[0][28]	= settr(27,2,39,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][36]	= settr(35,2,39,1,0,".(goto)", 1, 2500, 0);
	trans[0][29]	= settr(28,2,31,192,0,"(((global_state.actuator.x>10)==0))", 1, 2500, 0);
	T = trans[ 0][31] = settr(30,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(30,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][30]	= settr(29,2,39,193,193,"Modemanager_feedback_channel!0", 1, 2500, 0); /* m: 32 -> 39,0 */
	reached0[32] = 1;
	trans[0][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][33]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[0][34]	= settr(33,2,39,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][39]	= settr(38,2,40,1,0,"(1)", 1, 2500, 0);
	trans[0][42]	= settr(41,2,89,1,0,"break", 1, 2500, 0);
	trans[0][84]	= settr(83,2,89,1,0,".(goto)", 1, 2500, 0);
	trans[0][44]	= settr(43,2,48,194,0,"((global_state.actuator.state==1))", 1, 2500, 0);
	T = trans[ 0][48] = settr(47,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(47,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][45]	= settr(44,2,47,195,195,"global_state.actuator.x = Actuator_ctrl_signal_parameter", 1, 2500, 0);
	T = trans[ 0][47] = settr(46,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(46,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][46]	= settr(45,2,80,196,0,"assert(((global_state.actuator.x>=0)&&(global_state.actuator.x<=1000000)))", 1, 2500, 0);
	T = trans[ 0][80] = settr(79,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(79,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][49]	= settr(48,2,77,197,197,"transition_id = 0", 1, 2500, 0); /* m: 50 -> 0,77 */
	reached0[50] = 1;
	trans[0][50]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[0][78]	= settr(77,2,77,1,0,".(goto)", 1, 2500, 0);
	T = trans[0][77] = settr(76,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(76,2,74,0,0,"DO", 1, 2500, 0);
	T = trans[0][74] = settr(73,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(73,2,51,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(73,2,53,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(73,2,57,0,0,"IF", 1, 2500, 0);
	trans[0][51]	= settr(50,2,89,198,198,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][52]	= settr(51,2,89,1,0,"goto :b2", 1, 2500, 0);
	trans[0][75]	= settr(74,2,76,1,0,".(goto)", 1, 2500, 0);
	trans[0][53]	= settr(52,2,76,199,199,"((transition_id==0))", 1, 2500, 0); /* m: 54 -> 76,0 */
	reached0[54] = 1;
	trans[0][54]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][55]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[0][56]	= settr(55,2,76,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][57]	= settr(56,2,59,200,200,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][59] = settr(58,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(58,0,58,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][58]	= settr(57,2,72,1,0,"(1)", 1, 2500, 0);
	T = trans[0][72] = settr(71,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(71,2,60,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(71,2,66,0,0,"IF", 1, 2500, 0);
	trans[0][60]	= settr(59,2,62,201,0,"(((global_state.actuator.x>10)==1))", 1, 2500, 0);
	T = trans[ 0][62] = settr(61,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(61,0,61,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][61]	= settr(60,2,76,202,202,"Modemanager_feedback_channel!global_state.actuator.x", 1, 2500, 0); /* m: 63 -> 76,0 */
	reached0[63] = 1;
	trans[0][63]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][64]	= settr(0,0,0,0,0,"global_state.actuator.state = 1",0,0,0);
	trans[0][65]	= settr(64,2,76,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][73]	= settr(72,2,76,1,0,".(goto)", 1, 2500, 0);
	trans[0][66]	= settr(65,2,68,203,0,"(((global_state.actuator.x>10)==0))", 1, 2500, 0);
	T = trans[ 0][68] = settr(67,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(67,0,67,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][67]	= settr(66,2,76,204,204,"Modemanager_feedback_channel!0", 1, 2500, 0); /* m: 69 -> 76,0 */
	reached0[69] = 1;
	trans[0][69]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][70]	= settr(0,0,0,0,0,"global_state.actuator.state = 0",0,0,0);
	trans[0][71]	= settr(70,2,76,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][76]	= settr(75,2,77,1,0,"(1)", 1, 2500, 0);
	trans[0][79]	= settr(78,2,89,1,0,"break", 1, 2500, 0);
	trans[0][81]	= settr(80,2,82,2,0,"else", 1, 2500, 0);
	trans[0][82]	= settr(81,2,89,1,0,"(1)", 1, 2500, 0);
	trans[0][86]	= settr(85,2,89,1,0,"goto Actuator_ctrl_loop", 1, 2500, 0);
	trans[0][90]	= settr(89,2,91,1,0,".(goto)", 1, 2500, 0);
	trans[0][87]	= settr(86,2,88,205,0,"(empty(Actuator_ctrl_channel))", 1, 2500, 0);
	trans[0][88]	= settr(87,2,91,206,206,"(1)", 1, 2500, 0); /* m: 90 -> 91,0 */
	reached0[90] = 1;
	trans[0][91]	= settr(90,0,93,207,207,"Actuator_lock!1", 1, 2500, 0);
	trans[0][95]	= settr(94,0,96,1,0,"break", 0, 2, 0);
	trans[0][96]	= settr(95,0,0,208,208,"-end-", 0, 3500, 0);
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
