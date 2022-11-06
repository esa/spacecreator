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

	trans = (Trans ***) emalloc(12*sizeof(Trans **));

	/* proctype 10: never_0 */

	trans[10] = (Trans **) emalloc(29*sizeof(Trans *));

	trans[10][6]	= settr(2354,0,5,1,0,".(goto)", 0, 2, 0);
	T = trans[10][5] = settr(2353,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2353,0,1,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(2353,0,3,0,0,"DO", 0, 2, 0);
	trans[10][1]	= settr(2349,0,16,3,0,"(inited)", 1, 2, 0);
	trans[10][2]	= settr(2350,0,16,1,0,"goto :b41", 0, 2, 0);
	trans[10][3]	= settr(2351,0,5,2,0,"else", 0, 2, 0);
	trans[10][4]	= settr(2352,0,5,1,0,"goto start", 0, 2, 0);
	trans[10][7]	= settr(2355,0,16,1,0,"break", 0, 2, 0);
	trans[10][17]	= settr(2365,0,16,1,0,".(goto)", 0, 2, 0);
	T = trans[10][16] = settr(2364,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2364,0,11,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2364,0,12,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(2364,0,14,0,0,"DO", 0, 2, 0);
	T = trans[ 10][11] = settr(2359,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(2359,2,8,0,0,"ATOMIC", 1, 2, 0);
	trans[10][8]	= settr(2356,0,16,4,4,"((global_state.untitled_msc.state==2))", 1, 2, 0); /* m: 9 -> 16,0 */
	reached10[9] = 1;
	trans[10][9]	= settr(0,0,0,0,0,"printf('Observer entered errorstate: untitled_msc\\n')",0,0,0);
	trans[10][10]	= settr(0,0,0,0,0,"assert(!((global_state.untitled_msc.state==2)))",0,0,0);
	trans[10][12]	= settr(2360,0,25,5,0,"((global_state.untitled_msc.state==3))", 1, 2, 0);
	trans[10][13]	= settr(2361,0,25,1,0,"goto state_0", 0, 2, 0);
	trans[10][14]	= settr(2362,0,16,2,0,"else", 0, 2, 0);
	trans[10][15]	= settr(2363,0,16,1,0,"goto accept_all", 0, 2, 0);
	trans[10][18]	= settr(2366,0,25,1,0,"break", 0, 2, 0);
	trans[10][26]	= settr(2374,0,25,1,0,".(goto)", 0, 2, 0);
	T = trans[10][25] = settr(2373,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2373,0,22,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(2373,0,23,0,0,"DO", 0, 2, 0);
	T = trans[ 10][22] = settr(2370,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(2370,2,19,0,0,"ATOMIC", 1, 2, 0);
	trans[10][19]	= settr(2367,0,25,6,6,"((global_state.untitled_msc.state==2))", 1, 2, 0); /* m: 20 -> 25,0 */
	reached10[20] = 1;
	trans[10][20]	= settr(0,0,0,0,0,"printf('Observer entered errorstate: untitled_msc\\n')",0,0,0);
	trans[10][21]	= settr(0,0,0,0,0,"assert(!((global_state.untitled_msc.state==2)))",0,0,0);
	trans[10][23]	= settr(2371,0,25,2,0,"else", 0, 2, 0);
	trans[10][24]	= settr(2372,0,25,1,0,"goto state_0", 0, 2, 0);
	trans[10][27]	= settr(2375,0,28,1,0,"break", 0, 2, 0);
	trans[10][28]	= settr(2376,0,0,7,7,"-end-", 0, 3500, 0);

	/* proctype 9: :init: */

	trans[9] = (Trans **) emalloc(342*sizeof(Trans *));

	T = trans[ 9][340] = settr(2347,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(2347,2,5,0,0,"ATOMIC", 1, 2500, 0);
	T = trans[ 9][5] = settr(2012,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2012,0,4,0,0,"sub-sequence", 1, 2500, 0);
/*->*/	trans[9][4]	= settr(2011,34,154,8,8,"D_STEP20551", 1, 2, 0);
	T = trans[ 9][154] = settr(2161,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2161,0,9,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][9] = settr(2016,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2016,0,6,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][6]	= settr(2013,2,8,9,9,"global_state.modemanager.sender = 6", 1, 2500, 0);
	T = trans[ 9][8] = settr(2015,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2015,0,7,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][7]	= settr(2014,2,153,10,0,"assert((((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4))||(global_state.modemanager.sender==5))||(global_state.modemanager.sender==6)))", 1, 2500, 0);
	T = trans[ 9][153] = settr(2160,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2160,0,10,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][10]	= settr(2017,2,150,11,11,"transition_id = 0", 1, 2500, 0); /* m: 11 -> 0,150 */
	reached9[11] = 1;
	trans[9][11]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][151]	= settr(2158,2,150,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][150] = settr(2157,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2157,2,147,0,0,"DO", 1, 2500, 0);
	T = trans[9][147] = settr(2154,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2154,2,12,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2154,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2154,2,18,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2154,2,60,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2154,2,94,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2154,2,115,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2154,2,119,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2154,2,131,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2154,2,135,0,0,"IF", 1, 2500, 0);
	trans[9][12]	= settr(2019,2,155,12,12,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][13]	= settr(2020,2,155,1,0,"goto :b37", 1, 2500, 0);
	trans[9][148]	= settr(2155,2,149,1,0,".(goto)", 1, 2500, 0);
	trans[9][14]	= settr(2021,2,149,13,13,"((transition_id==0))", 1, 2500, 0); /* m: 15 -> 149,0 */
	reached9[15] = 1;
	trans[9][15]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][16]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[9][17]	= settr(2024,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][18]	= settr(2025,2,20,14,14,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][20] = settr(2027,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2027,0,19,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][19]	= settr(2026,2,58,1,0,"(1)", 1, 2500, 0);
	T = trans[9][58] = settr(2065,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2065,2,21,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2065,2,37,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2065,2,55,0,0,"IF", 1, 2500, 0);
	trans[9][21]	= settr(2028,2,25,15,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 9][25] = settr(2032,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2032,0,22,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][22]	= settr(2029,2,24,16,16,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 9][24] = settr(2031,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2031,0,23,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][23]	= settr(2030,2,29,17,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 9][29] = settr(2036,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2036,0,26,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][26]	= settr(2033,2,28,18,18,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 9][28] = settr(2035,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2035,0,27,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][27]	= settr(2034,2,31,19,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 9][31] = settr(2038,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2038,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][30]	= settr(2037,2,33,20,20,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 9][33] = settr(2040,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2040,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][32]	= settr(2039,2,149,21,21,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 34 -> 149,0 */
	reached9[34] = 1;
	trans[9][34]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][35]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[9][36]	= settr(2043,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][59]	= settr(2066,2,149,1,0,".(goto)", 1, 2500, 0);
	trans[9][37]	= settr(2044,2,50,22,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 9][50] = settr(2057,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2057,0,41,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][41] = settr(2048,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2048,0,38,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][38]	= settr(2045,2,40,23,23,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 9][40] = settr(2047,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2047,0,39,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][39]	= settr(2046,2,45,24,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 9][45] = settr(2052,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2052,0,42,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][42]	= settr(2049,2,44,25,25,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 9][44] = settr(2051,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2051,0,43,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][43]	= settr(2050,2,49,26,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 9][49] = settr(2056,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2056,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][46]	= settr(2053,2,48,27,27,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 9][48] = settr(2055,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2055,0,47,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][47]	= settr(2054,2,52,28,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 9][52] = settr(2059,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2059,0,51,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][51]	= settr(2058,2,149,29,29,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 53 -> 149,0 */
	reached9[53] = 1;
	trans[9][53]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][54]	= settr(2061,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][55]	= settr(2062,2,56,2,0,"else", 1, 2500, 0);
	trans[9][56]	= settr(2063,2,149,30,30,"transition_id = -(1)", 1, 2500, 0);
	trans[9][57]	= settr(2064,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][60]	= settr(2067,2,62,31,31,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][62] = settr(2069,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2069,0,61,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][61]	= settr(2068,2,92,1,0,"(1)", 1, 2500, 0);
	T = trans[9][92] = settr(2099,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2099,2,63,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2099,2,71,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2099,2,89,0,0,"IF", 1, 2500, 0);
	trans[9][63]	= settr(2070,2,65,32,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 9][65] = settr(2072,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2072,0,64,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][64]	= settr(2071,2,67,33,33,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 9][67] = settr(2074,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2074,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][66]	= settr(2073,2,149,34,34,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 68 -> 149,0 */
	reached9[68] = 1;
	trans[9][68]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][69]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[9][70]	= settr(2077,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][93]	= settr(2100,2,149,1,0,".(goto)", 1, 2500, 0);
	trans[9][71]	= settr(2078,2,84,35,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 9][84] = settr(2091,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2091,0,75,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][75] = settr(2082,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2082,0,72,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][72]	= settr(2079,2,74,36,36,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 9][74] = settr(2081,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2081,0,73,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][73]	= settr(2080,2,79,37,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 9][79] = settr(2086,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2086,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][76]	= settr(2083,2,78,38,38,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 9][78] = settr(2085,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2085,0,77,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][77]	= settr(2084,2,83,39,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 9][83] = settr(2090,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2090,0,80,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][80]	= settr(2087,2,82,40,40,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 9][82] = settr(2089,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2089,0,81,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][81]	= settr(2088,2,86,41,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 9][86] = settr(2093,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2093,0,85,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][85]	= settr(2092,2,149,42,42,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 87 -> 149,0 */
	reached9[87] = 1;
	trans[9][87]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][88]	= settr(2095,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][89]	= settr(2096,2,90,2,0,"else", 1, 2500, 0);
	trans[9][90]	= settr(2097,2,149,43,43,"transition_id = -(1)", 1, 2500, 0);
	trans[9][91]	= settr(2098,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][94]	= settr(2101,2,96,44,44,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][96] = settr(2103,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2103,0,95,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][95]	= settr(2102,2,103,1,0,"(1)", 1, 2500, 0);
	T = trans[9][103] = settr(2110,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2110,2,97,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2110,2,100,0,0,"IF", 1, 2500, 0);
	trans[9][97]	= settr(2104,2,99,45,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 9][99] = settr(2106,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2106,0,98,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][98]	= settr(2105,2,111,46,46,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[9][104]	= settr(2111,2,111,1,0,".(goto)", 1, 2500, 0);
	trans[9][100]	= settr(2107,2,102,2,0,"else", 1, 2500, 0);
	T = trans[ 9][102] = settr(2109,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2109,0,101,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][101]	= settr(2108,2,111,47,47,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[9][111] = settr(2118,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2118,2,105,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2118,2,108,0,0,"IF", 1, 2500, 0);
	trans[9][105]	= settr(2112,2,107,48,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 9][107] = settr(2114,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2114,0,106,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][106]	= settr(2113,2,149,49,49,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 113 -> 149,0 */
	reached9[113] = 1;
	trans[9][112]	= settr(2119,2,113,1,0,".(goto)", 1, 2500, 0); /* m: 113 -> 0,149 */
	reached9[113] = 1;
	trans[9][108]	= settr(2115,2,110,2,0,"else", 1, 2500, 0);
	T = trans[ 9][110] = settr(2117,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2117,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][109]	= settr(2116,2,149,50,50,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 113 -> 149,0 */
	reached9[113] = 1;
	trans[9][113]	= settr(2120,2,149,51,51,"transition_id = -(1)", 1, 2500, 0);
	trans[9][114]	= settr(2121,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][115]	= settr(2122,2,117,52,52,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 9][117] = settr(2124,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2124,0,116,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][116]	= settr(2123,2,129,1,0,"(1)", 1, 2500, 0);
	trans[9][118]	= settr(2125,2,129,1,0,"goto startcheck", 1, 2500, 0);
	trans[9][119]	= settr(2126,2,121,53,53,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 9][121] = settr(2128,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2128,0,120,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][120]	= settr(2127,2,129,1,0,"(1)", 1, 2500, 0);
	T = trans[9][129] = settr(2136,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2136,2,122,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2136,2,125,0,0,"IF", 1, 2500, 0);
	trans[9][122]	= settr(2129,2,149,54,54,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 123 -> 149,0 */
	reached9[123] = 1;
	trans[9][123]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][124]	= settr(2131,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][130]	= settr(2137,2,149,1,0,".(goto)", 1, 2500, 0);
	trans[9][125]	= settr(2132,2,149,55,55,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 126 -> 149,0 */
	reached9[126] = 1;
	trans[9][126]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][127]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[9][128]	= settr(2135,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][131]	= settr(2138,2,133,56,56,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 9][133] = settr(2140,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2140,0,132,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][132]	= settr(2139,2,145,1,0,"(1)", 1, 2500, 0);
	trans[9][134]	= settr(2141,2,145,1,0,"goto stopcheck", 1, 2500, 0);
	trans[9][135]	= settr(2142,2,137,57,57,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 9][137] = settr(2144,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2144,0,136,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][136]	= settr(2143,2,145,1,0,"(1)", 1, 2500, 0);
	T = trans[9][145] = settr(2152,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2152,2,138,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2152,2,141,0,0,"IF", 1, 2500, 0);
	trans[9][138]	= settr(2145,2,149,58,58,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 139 -> 149,0 */
	reached9[139] = 1;
	trans[9][139]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][140]	= settr(2147,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][146]	= settr(2153,2,149,1,0,".(goto)", 1, 2500, 0);
	trans[9][141]	= settr(2148,2,149,59,59,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 142 -> 149,0 */
	reached9[142] = 1;
	trans[9][142]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][143]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[9][144]	= settr(2151,2,149,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][149]	= settr(2156,2,150,1,0,"(1)", 1, 2500, 0);
	trans[9][152]	= settr(2159,2,155,1,0,"break", 1, 2500, 0);
	trans[9][155]	= settr(2162,2,223,60,60,"Modemanager_lock!1", 1, 2500, 0);
	T = trans[ 9][223] = settr(2230,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2230,0,159,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][159] = settr(2166,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2166,0,156,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][156]	= settr(2163,2,158,61,61,"global_state.actuator1.sender = 6", 1, 2500, 0);
	T = trans[ 9][158] = settr(2165,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2165,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][157]	= settr(2164,2,222,62,0,"assert((((((((global_state.actuator1.sender==0)||(global_state.actuator1.sender==1))||(global_state.actuator1.sender==2))||(global_state.actuator1.sender==3))||(global_state.actuator1.sender==4))||(global_state.actuator1.sender==5))||(global_state.actuator1.sender==6)))", 1, 2500, 0);
	T = trans[ 9][222] = settr(2229,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2229,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][160]	= settr(2167,2,219,63,63,"transition_id = 0", 1, 2500, 0); /* m: 161 -> 0,219 */
	reached9[161] = 1;
	trans[9][161]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][220]	= settr(2227,2,219,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][219] = settr(2226,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2226,2,216,0,0,"DO", 1, 2500, 0);
	T = trans[9][216] = settr(2223,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2223,2,162,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2223,2,164,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2223,2,168,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2223,2,188,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2223,2,202,0,0,"IF", 1, 2500, 0);
	trans[9][162]	= settr(2169,2,224,64,64,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][163]	= settr(2170,2,224,1,0,"goto :b38", 1, 2500, 0);
	trans[9][217]	= settr(2224,2,218,1,0,".(goto)", 1, 2500, 0);
	trans[9][164]	= settr(2171,2,218,65,65,"((transition_id==0))", 1, 2500, 0); /* m: 165 -> 218,0 */
	reached9[165] = 1;
	trans[9][165]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][166]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][167]	= settr(2174,2,218,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][168]	= settr(2175,2,170,66,66,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][170] = settr(2177,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2177,0,169,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][169]	= settr(2176,2,186,1,0,"(1)", 1, 2500, 0);
	T = trans[9][186] = settr(2193,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2193,2,171,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2193,2,177,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2193,2,183,0,0,"IF", 1, 2500, 0);
	trans[9][171]	= settr(2178,2,173,67,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 9][173] = settr(2180,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2180,0,172,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][172]	= settr(2179,2,218,68,68,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 174 -> 218,0 */
	reached9[174] = 1;
	trans[9][174]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][175]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][176]	= settr(2183,2,218,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][187]	= settr(2194,2,218,1,0,".(goto)", 1, 2500, 0);
	trans[9][177]	= settr(2184,2,179,69,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 9][179] = settr(2186,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2186,0,178,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][178]	= settr(2185,2,218,70,70,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 180 -> 218,0 */
	reached9[180] = 1;
	trans[9][180]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][181]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[9][182]	= settr(2189,2,218,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][183]	= settr(2190,2,184,2,0,"else", 1, 2500, 0);
	trans[9][184]	= settr(2191,2,218,71,71,"transition_id = -(1)", 1, 2500, 0);
	trans[9][185]	= settr(2192,2,218,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][188]	= settr(2195,2,190,72,72,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][190] = settr(2197,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2197,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][189]	= settr(2196,2,200,1,0,"(1)", 1, 2500, 0);
	T = trans[9][200] = settr(2207,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2207,2,191,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2207,2,197,0,0,"IF", 1, 2500, 0);
	trans[9][191]	= settr(2198,2,193,73,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 9][193] = settr(2200,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2200,0,192,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][192]	= settr(2199,2,218,74,74,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 194 -> 218,0 */
	reached9[194] = 1;
	trans[9][194]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][195]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][196]	= settr(2203,2,218,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][201]	= settr(2208,2,218,1,0,".(goto)", 1, 2500, 0);
	trans[9][197]	= settr(2204,2,198,2,0,"else", 1, 2500, 0);
	trans[9][198]	= settr(2205,2,218,75,75,"transition_id = -(1)", 1, 2500, 0);
	trans[9][199]	= settr(2206,2,218,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][202]	= settr(2209,2,204,76,76,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][204] = settr(2211,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2211,0,203,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][203]	= settr(2210,2,214,1,0,"(1)", 1, 2500, 0);
	T = trans[9][214] = settr(2221,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2221,2,205,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2221,2,211,0,0,"IF", 1, 2500, 0);
	trans[9][205]	= settr(2212,2,207,77,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 9][207] = settr(2214,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2214,0,206,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][206]	= settr(2213,2,218,78,78,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 208 -> 218,0 */
	reached9[208] = 1;
	trans[9][208]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][209]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[9][210]	= settr(2217,2,218,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][215]	= settr(2222,2,218,1,0,".(goto)", 1, 2500, 0);
	trans[9][211]	= settr(2218,2,212,2,0,"else", 1, 2500, 0);
	trans[9][212]	= settr(2219,2,218,79,79,"transition_id = -(1)", 1, 2500, 0);
	trans[9][213]	= settr(2220,2,218,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][218]	= settr(2225,2,219,1,0,"(1)", 1, 2500, 0);
	trans[9][221]	= settr(2228,2,224,1,0,"break", 1, 2500, 0);
	trans[9][224]	= settr(2231,2,292,80,80,"Actuator1_lock!1", 1, 2500, 0);
	T = trans[ 9][292] = settr(2299,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2299,0,228,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][228] = settr(2235,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2235,0,225,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][225]	= settr(2232,2,227,81,81,"global_state.actuator2.sender = 6", 1, 2500, 0);
	T = trans[ 9][227] = settr(2234,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2234,0,226,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][226]	= settr(2233,2,291,82,0,"assert((((((((global_state.actuator2.sender==0)||(global_state.actuator2.sender==1))||(global_state.actuator2.sender==2))||(global_state.actuator2.sender==3))||(global_state.actuator2.sender==4))||(global_state.actuator2.sender==5))||(global_state.actuator2.sender==6)))", 1, 2500, 0);
	T = trans[ 9][291] = settr(2298,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2298,0,229,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][229]	= settr(2236,2,288,83,83,"transition_id = 0", 1, 2500, 0); /* m: 230 -> 0,288 */
	reached9[230] = 1;
	trans[9][230]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][289]	= settr(2296,2,288,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][288] = settr(2295,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2295,2,285,0,0,"DO", 1, 2500, 0);
	T = trans[9][285] = settr(2292,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2292,2,231,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2292,2,233,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2292,2,237,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2292,2,257,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2292,2,271,0,0,"IF", 1, 2500, 0);
	trans[9][231]	= settr(2238,2,293,84,84,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][232]	= settr(2239,2,293,1,0,"goto :b39", 1, 2500, 0);
	trans[9][286]	= settr(2293,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][233]	= settr(2240,2,287,85,85,"((transition_id==0))", 1, 2500, 0); /* m: 234 -> 287,0 */
	reached9[234] = 1;
	trans[9][234]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][235]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][236]	= settr(2243,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][237]	= settr(2244,2,239,86,86,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][239] = settr(2246,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2246,0,238,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][238]	= settr(2245,2,255,1,0,"(1)", 1, 2500, 0);
	T = trans[9][255] = settr(2262,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2262,2,240,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2262,2,246,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2262,2,252,0,0,"IF", 1, 2500, 0);
	trans[9][240]	= settr(2247,2,242,87,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 9][242] = settr(2249,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2249,0,241,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][241]	= settr(2248,2,287,88,88,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 243 -> 287,0 */
	reached9[243] = 1;
	trans[9][243]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][244]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][245]	= settr(2252,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][256]	= settr(2263,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][246]	= settr(2253,2,248,89,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 9][248] = settr(2255,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2255,0,247,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][247]	= settr(2254,2,287,90,90,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 249 -> 287,0 */
	reached9[249] = 1;
	trans[9][249]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][250]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[9][251]	= settr(2258,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][252]	= settr(2259,2,253,2,0,"else", 1, 2500, 0);
	trans[9][253]	= settr(2260,2,287,91,91,"transition_id = -(1)", 1, 2500, 0);
	trans[9][254]	= settr(2261,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][257]	= settr(2264,2,259,92,92,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][259] = settr(2266,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2266,0,258,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][258]	= settr(2265,2,269,1,0,"(1)", 1, 2500, 0);
	T = trans[9][269] = settr(2276,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2276,2,260,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2276,2,266,0,0,"IF", 1, 2500, 0);
	trans[9][260]	= settr(2267,2,262,93,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 9][262] = settr(2269,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2269,0,261,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][261]	= settr(2268,2,287,94,94,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 263 -> 287,0 */
	reached9[263] = 1;
	trans[9][263]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][264]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][265]	= settr(2272,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][270]	= settr(2277,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][266]	= settr(2273,2,267,2,0,"else", 1, 2500, 0);
	trans[9][267]	= settr(2274,2,287,95,95,"transition_id = -(1)", 1, 2500, 0);
	trans[9][268]	= settr(2275,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][271]	= settr(2278,2,273,96,96,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][273] = settr(2280,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2280,0,272,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][272]	= settr(2279,2,283,1,0,"(1)", 1, 2500, 0);
	T = trans[9][283] = settr(2290,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2290,2,274,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2290,2,280,0,0,"IF", 1, 2500, 0);
	trans[9][274]	= settr(2281,2,276,97,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 9][276] = settr(2283,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2283,0,275,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][275]	= settr(2282,2,287,98,98,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 277 -> 287,0 */
	reached9[277] = 1;
	trans[9][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][278]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[9][279]	= settr(2286,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][284]	= settr(2291,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][280]	= settr(2287,2,281,2,0,"else", 1, 2500, 0);
	trans[9][281]	= settr(2288,2,287,99,99,"transition_id = -(1)", 1, 2500, 0);
	trans[9][282]	= settr(2289,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][287]	= settr(2294,2,288,1,0,"(1)", 1, 2500, 0);
	trans[9][290]	= settr(2297,2,293,1,0,"break", 1, 2500, 0);
	trans[9][293]	= settr(2300,2,337,100,100,"Actuator2_lock!1", 1, 2500, 0);
	T = trans[ 9][337] = settr(2344,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2344,0,297,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][297] = settr(2304,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2304,0,294,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][294]	= settr(2301,2,296,101,101,"global_state.untitled_msc.sender = 6", 1, 2500, 0);
	T = trans[ 9][296] = settr(2303,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2303,0,295,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][295]	= settr(2302,2,336,102,0,"assert((((((((global_state.untitled_msc.sender==0)||(global_state.untitled_msc.sender==1))||(global_state.untitled_msc.sender==2))||(global_state.untitled_msc.sender==3))||(global_state.untitled_msc.sender==4))||(global_state.untitled_msc.sender==5))||(global_state.untitled_msc.sender==6)))", 1, 2500, 0);
	T = trans[ 9][336] = settr(2343,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2343,0,298,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][298]	= settr(2305,2,333,103,103,"transition_id = 0", 1, 2500, 0); /* m: 299 -> 0,333 */
	reached9[299] = 1;
	trans[9][299]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][334]	= settr(2341,2,333,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][333] = settr(2340,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2340,2,330,0,0,"DO", 1, 2500, 0);
	T = trans[9][330] = settr(2337,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2337,2,300,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2337,2,302,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2337,2,306,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2337,2,318,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2337,2,322,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2337,2,326,0,0,"IF", 1, 2500, 0);
	trans[9][300]	= settr(2307,2,338,104,104,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][301]	= settr(2308,2,338,1,0,"goto :b40", 1, 2500, 0);
	trans[9][331]	= settr(2338,2,332,1,0,".(goto)", 1, 2500, 0);
	trans[9][302]	= settr(2309,2,332,105,105,"((transition_id==0))", 1, 2500, 0); /* m: 303 -> 332,0 */
	reached9[303] = 1;
	trans[9][303]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][304]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[9][305]	= settr(2312,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][306]	= settr(2313,2,313,106,106,"((transition_id==1))", 1, 2500, 0);
	T = trans[9][313] = settr(2320,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2320,2,307,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2320,2,311,0,0,"IF", 1, 2500, 0);
	trans[9][307]	= settr(2314,2,332,107,107,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 308 -> 332,0 */
	reached9[308] = 1;
	trans[9][308]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][309]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[9][310]	= settr(2317,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][314]	= settr(2321,2,315,1,0,".(goto)", 1, 2500, 0); /* m: 315 -> 0,332 */
	reached9[315] = 1;
	trans[9][311]	= settr(2318,2,312,2,0,"else", 1, 2500, 0);
	trans[9][312]	= settr(2319,2,332,108,108,"(1)", 1, 2500, 0); /* m: 315 -> 332,0 */
	reached9[315] = 1;
	trans[9][315]	= settr(2322,2,332,109,109,"transition_id = -(1)", 1, 2500, 0); /* m: 316 -> 0,332 */
	reached9[316] = 1;
	trans[9][316]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[9][317]	= settr(2324,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][318]	= settr(2325,2,332,110,110,"((transition_id==2))", 1, 2500, 0); /* m: 319 -> 332,0 */
	reached9[319] = 1;
	trans[9][319]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][320]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[9][321]	= settr(2328,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][322]	= settr(2329,2,332,111,111,"((transition_id==3))", 1, 2500, 0); /* m: 323 -> 332,0 */
	reached9[323] = 1;
	trans[9][323]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][324]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[9][325]	= settr(2332,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][326]	= settr(2333,2,332,112,112,"((transition_id==4))", 1, 2500, 0); /* m: 327 -> 332,0 */
	reached9[327] = 1;
	trans[9][327]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][328]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[9][329]	= settr(2336,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][332]	= settr(2339,2,333,1,0,"(1)", 1, 2500, 0);
	trans[9][335]	= settr(2342,2,338,1,0,"break", 1, 2500, 0);
	trans[9][338]	= settr(2345,4,341,113,113,"Untitled_msc_lock!1", 1, 2500, 0); /* m: 339 -> 341,0 */
	reached9[339] = 1;
	trans[9][339]	= settr(0,0,0,0,0,"inited = 1",0,0,0);
	trans[9][341]	= settr(2348,0,0,114,114,"-end-", 0, 3500, 0);

	/* proctype 8: Sensor_reading */

	trans[8] = (Trans **) emalloc(45*sizeof(Trans *));

	trans[8][1]	= settr(1964,0,2,115,0,"(inited)", 1, 2, 0);
	trans[8][2]	= settr(1965,0,41,116,116,"value = 0", 0, 2, 0); /* m: 3 -> 0,41 */
	reached8[3] = 1;
	trans[8][3]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[8][4]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[8][42]	= settr(2005,0,41,1,0,".(goto)", 0, 2, 0);
	T = trans[8][41] = settr(2004,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2004,0,5,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(2004,0,39,0,0,"DO", 0, 2, 0);
	trans[8][5]	= settr(1968,0,37,117,0,"((inputVectorCounter<=1))", 0, 2, 0);
	T = trans[ 8][37] = settr(2000,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(2000,2,34,0,0,"ATOMIC", 1, 2, 0);
	T = trans[ 8][34] = settr(1997,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1997,0,6,0,0,"sub-sequence", 1, 2, 0);
	trans[8][6]	= settr(1969,2,31,118,118,"Sensor_reading_x_tmp = 0", 1, 2, 0);
	T = trans[8][31] = settr(1994,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(1994,2,14,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(1994,2,22,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(1994,2,30,0,0,"IF", 1, 2, 0);
	T = trans[ 8][14] = settr(1977,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1977,0,7,0,0,"sub-sequence", 1, 2, 0);
	trans[8][7]	= settr(1970,2,11,119,119,"Sensor_reading_x_tmp = 10", 1, 2, 0);
	trans[8][12]	= settr(1975,2,11,1,0,".(goto)", 1, 2, 0);
	T = trans[8][11] = settr(1974,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(1974,2,10,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(1974,2,8,0,0,"DO", 1, 2, 0);
	trans[8][10]	= settr(1973,2,36,120,120,"goto :b34", 1, 2, 0); /* m: 33 -> 0,36 */
	reached8[33] = 1;
	trans[8][8]	= settr(1971,2,11,121,121,"((Sensor_reading_x_tmp<10))", 1, 2, 0); /* m: 9 -> 11,0 */
	reached8[9] = 1;
	trans[8][9]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[8][13]	= settr(1976,2,33,1,0,"break", 1, 2, 0);
	trans[8][32]	= settr(1995,2,33,1,0,".(goto)", 1, 2, 0); /* m: 33 -> 0,36 */
	reached8[33] = 1;
	T = trans[ 8][22] = settr(1985,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1985,0,15,0,0,"sub-sequence", 1, 2, 0);
	trans[8][15]	= settr(1978,2,19,122,122,"Sensor_reading_x_tmp = 50", 1, 2, 0);
	trans[8][20]	= settr(1983,2,19,1,0,".(goto)", 1, 2, 0);
	T = trans[8][19] = settr(1982,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(1982,2,18,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(1982,2,16,0,0,"DO", 1, 2, 0);
	trans[8][18]	= settr(1981,2,36,123,123,"goto :b35", 1, 2, 0); /* m: 33 -> 0,36 */
	reached8[33] = 1;
	trans[8][16]	= settr(1979,2,19,124,124,"((Sensor_reading_x_tmp<50))", 1, 2, 0); /* m: 17 -> 19,0 */
	reached8[17] = 1;
	trans[8][17]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[8][21]	= settr(1984,2,33,1,0,"break", 1, 2, 0);
	T = trans[ 8][30] = settr(1993,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1993,0,23,0,0,"sub-sequence", 1, 2, 0);
	trans[8][23]	= settr(1986,2,27,125,125,"Sensor_reading_x_tmp = 100", 1, 2, 0);
	trans[8][28]	= settr(1991,2,27,1,0,".(goto)", 1, 2, 0);
	T = trans[8][27] = settr(1990,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(1990,2,26,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(1990,2,24,0,0,"DO", 1, 2, 0);
	trans[8][26]	= settr(1989,2,36,126,126,"goto :b36", 1, 2, 0); /* m: 33 -> 0,36 */
	reached8[33] = 1;
	trans[8][24]	= settr(1987,2,27,127,127,"((Sensor_reading_x_tmp<100))", 1, 2, 0); /* m: 25 -> 27,0 */
	reached8[25] = 1;
	trans[8][25]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[8][29]	= settr(1992,2,33,1,0,"break", 1, 2, 0);
	trans[8][33]	= settr(1996,2,36,128,128,"value = Sensor_reading_x_tmp", 1, 2, 0);
	T = trans[ 8][36] = settr(1999,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1999,0,35,0,0,"sub-sequence", 1, 2, 0);
	trans[8][35]	= settr(1998,0,41,129,129,"Modemanager_reading_channel!value", 1, 8, 0); /* m: 38 -> 41,0 */
	reached8[38] = 1;
	trans[8][38]	= settr(0,0,0,0,0,"inputVectorCounter = (inputVectorCounter+1)",0,0,0);
	trans[8][39]	= settr(2002,0,44,2,0,"else", 0, 2, 0);
	trans[8][40]	= settr(2003,0,44,1,0,"goto :b33", 0, 2, 0);
	trans[8][43]	= settr(2006,0,44,1,0,"break", 0, 2, 0);
	trans[8][44]	= settr(2007,0,0,130,130,"-end-", 0, 3500, 0);

	/* proctype 7: Modemanager_systemTc */

	trans[7] = (Trans **) emalloc(478*sizeof(Trans *));

	trans[7][1]	= settr(1487,0,474,131,0,"(inited)", 1, 2, 0);
	trans[7][475]	= settr(1961,0,474,1,0,".(goto)", 0, 2, 0);
	T = trans[7][474] = settr(1960,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1960,0,473,0,0,"DO", 0, 2, 0);
	T = trans[ 7][473] = settr(1959,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1959,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[7][2]	= settr(1488,2,3,132,0,"(nempty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][3]	= settr(1489,2,470,133,133,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[7][470] = settr(1956,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1956,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1956,2,468,0,0,"IF", 1, 2500, 0);
	trans[7][4]	= settr(1490,2,5,134,0,"(nempty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][5]	= settr(1491,2,353,135,135,"Modemanager_systemTc_channel?Modemanager_systemtc_signal_parameter.data.goToIdle,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2,Modemanager_systemtc_signal_parameter.data.doReport,Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0); /* m: 6 -> 353,0 */
	reached7[6] = 1;
	trans[7][6]	= settr(0,0,0,0,0,"Modemanager_systemtc_channel_used = 1",0,0,0);
	T = trans[ 7][353] = settr(1839,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1839,0,351,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[7][351] = settr(1837,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1837,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1837,2,178,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1837,2,349,0,0,"IF", 1, 2500, 0);
	trans[7][7]	= settr(1493,2,33,136,0,"((global_state.modemanager.state==3))", 1, 2500, 0);
	T = trans[ 7][33] = settr(1519,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1519,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][8]	= settr(1494,2,31,137,137,"global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0);
	T = trans[7][31] = settr(1517,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1517,2,9,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1517,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1517,2,24,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1517,2,29,0,0,"IF", 1, 2500, 0);
	trans[7][9]	= settr(1495,2,13,138,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][13] = settr(1499,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1499,0,10,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][10]	= settr(1496,2,12,139,139,"global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle", 1, 2500, 0);
	T = trans[ 7][12] = settr(1498,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1498,0,11,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][11]	= settr(1497,2,177,140,0,"assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))", 1, 2500, 0);
	trans[7][32]	= settr(1518,2,177,1,0,".(goto)", 1, 2500, 0);
	trans[7][14]	= settr(1500,2,23,141,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][23] = settr(1509,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1509,0,18,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][18] = settr(1504,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1504,0,15,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][15]	= settr(1501,2,17,142,142,"global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][17] = settr(1503,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1503,0,16,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][16]	= settr(1502,2,22,143,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][22] = settr(1508,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1508,0,19,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][19]	= settr(1505,2,21,144,144,"global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][21] = settr(1507,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1507,0,20,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][20]	= settr(1506,2,177,145,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))", 1, 2500, 0);
	trans[7][24]	= settr(1510,2,28,146,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][28] = settr(1514,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1514,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][25]	= settr(1511,2,27,147,147,"global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport", 1, 2500, 0);
	T = trans[ 7][27] = settr(1513,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1513,0,26,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][26]	= settr(1512,2,177,148,0,"assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))", 1, 2500, 0);
	trans[7][29]	= settr(1515,2,30,2,0,"else", 1, 2500, 0);
	trans[7][30]	= settr(1516,2,177,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][177] = settr(1663,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1663,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][34]	= settr(1520,2,174,149,149,"transition_id = 0", 1, 2500, 0); /* m: 35 -> 0,174 */
	reached7[35] = 1;
	trans[7][35]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[7][175]	= settr(1661,2,174,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][174] = settr(1660,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1660,2,171,0,0,"DO", 1, 2500, 0);
	T = trans[7][171] = settr(1657,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1657,2,36,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1657,2,38,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1657,2,42,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1657,2,84,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1657,2,118,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1657,2,139,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1657,2,143,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1657,2,155,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1657,2,159,0,0,"IF", 1, 2500, 0);
	trans[7][36]	= settr(1522,2,354,150,150,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][37]	= settr(1523,2,354,1,0,"goto :b29", 1, 2500, 0);
	trans[7][172]	= settr(1658,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][38]	= settr(1524,2,173,151,151,"((transition_id==0))", 1, 2500, 0); /* m: 39 -> 173,0 */
	reached7[39] = 1;
	trans[7][39]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][40]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][41]	= settr(1527,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][42]	= settr(1528,2,44,152,152,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][44] = settr(1530,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1530,0,43,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][43]	= settr(1529,2,82,1,0,"(1)", 1, 2500, 0);
	T = trans[7][82] = settr(1568,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1568,2,45,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1568,2,61,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1568,2,79,0,0,"IF", 1, 2500, 0);
	trans[7][45]	= settr(1531,2,49,153,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][49] = settr(1535,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1535,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][46]	= settr(1532,2,48,154,154,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][48] = settr(1534,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1534,0,47,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][47]	= settr(1533,2,53,155,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][53] = settr(1539,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1539,0,50,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][50]	= settr(1536,2,52,156,156,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][52] = settr(1538,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1538,0,51,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][51]	= settr(1537,2,55,157,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 7][55] = settr(1541,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1541,0,54,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][54]	= settr(1540,2,57,158,158,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 7][57] = settr(1543,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1543,0,56,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][56]	= settr(1542,2,173,159,159,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 58 -> 173,0 */
	reached7[58] = 1;
	trans[7][58]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][59]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[7][60]	= settr(1546,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][83]	= settr(1569,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][61]	= settr(1547,2,74,160,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][74] = settr(1560,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1560,0,65,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][65] = settr(1551,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1551,0,62,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][62]	= settr(1548,2,64,161,161,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][64] = settr(1550,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1550,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][63]	= settr(1549,2,69,162,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][69] = settr(1555,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1555,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][66]	= settr(1552,2,68,163,163,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][68] = settr(1554,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1554,0,67,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][67]	= settr(1553,2,73,164,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][73] = settr(1559,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1559,0,70,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][70]	= settr(1556,2,72,165,165,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][72] = settr(1558,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1558,0,71,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][71]	= settr(1557,2,76,166,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][76] = settr(1562,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1562,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][75]	= settr(1561,2,173,167,167,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 77 -> 173,0 */
	reached7[77] = 1;
	trans[7][77]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][78]	= settr(1564,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][79]	= settr(1565,2,80,2,0,"else", 1, 2500, 0);
	trans[7][80]	= settr(1566,2,173,168,168,"transition_id = -(1)", 1, 2500, 0);
	trans[7][81]	= settr(1567,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][84]	= settr(1570,2,86,169,169,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 7][86] = settr(1572,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1572,0,85,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][85]	= settr(1571,2,116,1,0,"(1)", 1, 2500, 0);
	T = trans[7][116] = settr(1602,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1602,2,87,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1602,2,95,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1602,2,113,0,0,"IF", 1, 2500, 0);
	trans[7][87]	= settr(1573,2,89,170,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][89] = settr(1575,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1575,0,88,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][88]	= settr(1574,2,91,171,171,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 7][91] = settr(1577,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1577,0,90,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][90]	= settr(1576,2,173,172,172,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 92 -> 173,0 */
	reached7[92] = 1;
	trans[7][92]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][93]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[7][94]	= settr(1580,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][117]	= settr(1603,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][95]	= settr(1581,2,108,173,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][108] = settr(1594,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1594,0,99,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][99] = settr(1585,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1585,0,96,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][96]	= settr(1582,2,98,174,174,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][98] = settr(1584,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1584,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][97]	= settr(1583,2,103,175,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][103] = settr(1589,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1589,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][100]	= settr(1586,2,102,176,176,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][102] = settr(1588,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1588,0,101,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][101]	= settr(1587,2,107,177,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][107] = settr(1593,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1593,0,104,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][104]	= settr(1590,2,106,178,178,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][106] = settr(1592,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1592,0,105,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][105]	= settr(1591,2,110,179,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][110] = settr(1596,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1596,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][109]	= settr(1595,2,173,180,180,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 111 -> 173,0 */
	reached7[111] = 1;
	trans[7][111]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][112]	= settr(1598,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][113]	= settr(1599,2,114,2,0,"else", 1, 2500, 0);
	trans[7][114]	= settr(1600,2,173,181,181,"transition_id = -(1)", 1, 2500, 0);
	trans[7][115]	= settr(1601,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][118]	= settr(1604,2,120,182,182,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 7][120] = settr(1606,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1606,0,119,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][119]	= settr(1605,2,127,1,0,"(1)", 1, 2500, 0);
	T = trans[7][127] = settr(1613,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1613,2,121,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1613,2,124,0,0,"IF", 1, 2500, 0);
	trans[7][121]	= settr(1607,2,123,183,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 7][123] = settr(1609,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1609,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][122]	= settr(1608,2,135,184,184,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[7][128]	= settr(1614,2,135,1,0,".(goto)", 1, 2500, 0);
	trans[7][124]	= settr(1610,2,126,2,0,"else", 1, 2500, 0);
	T = trans[ 7][126] = settr(1612,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1612,0,125,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][125]	= settr(1611,2,135,185,185,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[7][135] = settr(1621,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1621,2,129,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1621,2,132,0,0,"IF", 1, 2500, 0);
	trans[7][129]	= settr(1615,2,131,186,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 7][131] = settr(1617,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1617,0,130,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][130]	= settr(1616,2,173,187,187,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 137 -> 173,0 */
	reached7[137] = 1;
	trans[7][136]	= settr(1622,2,137,1,0,".(goto)", 1, 2500, 0); /* m: 137 -> 0,173 */
	reached7[137] = 1;
	trans[7][132]	= settr(1618,2,134,2,0,"else", 1, 2500, 0);
	T = trans[ 7][134] = settr(1620,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1620,0,133,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][133]	= settr(1619,2,173,188,188,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 137 -> 173,0 */
	reached7[137] = 1;
	trans[7][137]	= settr(1623,2,173,189,189,"transition_id = -(1)", 1, 2500, 0);
	trans[7][138]	= settr(1624,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][139]	= settr(1625,2,141,190,190,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 7][141] = settr(1627,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1627,0,140,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][140]	= settr(1626,2,153,1,0,"(1)", 1, 2500, 0);
	trans[7][142]	= settr(1628,2,153,1,0,"goto startcheck", 1, 2500, 0);
	trans[7][143]	= settr(1629,2,145,191,191,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 7][145] = settr(1631,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1631,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][144]	= settr(1630,2,153,1,0,"(1)", 1, 2500, 0);
	T = trans[7][153] = settr(1639,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1639,2,146,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1639,2,149,0,0,"IF", 1, 2500, 0);
	trans[7][146]	= settr(1632,2,173,192,192,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 147 -> 173,0 */
	reached7[147] = 1;
	trans[7][147]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][148]	= settr(1634,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][154]	= settr(1640,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][149]	= settr(1635,2,173,193,193,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 150 -> 173,0 */
	reached7[150] = 1;
	trans[7][150]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][151]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[7][152]	= settr(1638,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][155]	= settr(1641,2,157,194,194,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 7][157] = settr(1643,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1643,0,156,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][156]	= settr(1642,2,169,1,0,"(1)", 1, 2500, 0);
	trans[7][158]	= settr(1644,2,169,1,0,"goto stopcheck", 1, 2500, 0);
	trans[7][159]	= settr(1645,2,161,195,195,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 7][161] = settr(1647,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1647,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][160]	= settr(1646,2,169,1,0,"(1)", 1, 2500, 0);
	T = trans[7][169] = settr(1655,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1655,2,162,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1655,2,165,0,0,"IF", 1, 2500, 0);
	trans[7][162]	= settr(1648,2,173,196,196,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 163 -> 173,0 */
	reached7[163] = 1;
	trans[7][163]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][164]	= settr(1650,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][170]	= settr(1656,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][165]	= settr(1651,2,173,197,197,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 166 -> 173,0 */
	reached7[166] = 1;
	trans[7][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][168]	= settr(1654,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][173]	= settr(1659,2,174,1,0,"(1)", 1, 2500, 0);
	trans[7][176]	= settr(1662,2,354,1,0,"break", 1, 2500, 0);
	trans[7][352]	= settr(1838,2,354,1,0,".(goto)", 1, 2500, 0);
	trans[7][178]	= settr(1664,2,204,198,0,"((global_state.modemanager.state==1))", 1, 2500, 0);
	T = trans[ 7][204] = settr(1690,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1690,0,179,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][179]	= settr(1665,2,202,199,199,"global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0);
	T = trans[7][202] = settr(1688,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1688,2,180,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1688,2,185,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1688,2,195,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1688,2,200,0,0,"IF", 1, 2500, 0);
	trans[7][180]	= settr(1666,2,184,200,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][184] = settr(1670,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1670,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][181]	= settr(1667,2,183,201,201,"global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle", 1, 2500, 0);
	T = trans[ 7][183] = settr(1669,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1669,0,182,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][182]	= settr(1668,2,348,202,0,"assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))", 1, 2500, 0);
	trans[7][203]	= settr(1689,2,348,1,0,".(goto)", 1, 2500, 0);
	trans[7][185]	= settr(1671,2,194,203,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][194] = settr(1680,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1680,0,189,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][189] = settr(1675,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1675,0,186,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][186]	= settr(1672,2,188,204,204,"global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][188] = settr(1674,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1674,0,187,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][187]	= settr(1673,2,193,205,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][193] = settr(1679,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1679,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][190]	= settr(1676,2,192,206,206,"global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][192] = settr(1678,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1678,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][191]	= settr(1677,2,348,207,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))", 1, 2500, 0);
	trans[7][195]	= settr(1681,2,199,208,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][199] = settr(1685,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1685,0,196,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][196]	= settr(1682,2,198,209,209,"global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport", 1, 2500, 0);
	T = trans[ 7][198] = settr(1684,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1684,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][197]	= settr(1683,2,348,210,0,"assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))", 1, 2500, 0);
	trans[7][200]	= settr(1686,2,201,2,0,"else", 1, 2500, 0);
	trans[7][201]	= settr(1687,2,348,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][348] = settr(1834,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1834,0,205,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][205]	= settr(1691,2,345,211,211,"transition_id = 0", 1, 2500, 0); /* m: 206 -> 0,345 */
	reached7[206] = 1;
	trans[7][206]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[7][346]	= settr(1832,2,345,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][345] = settr(1831,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1831,2,342,0,0,"DO", 1, 2500, 0);
	T = trans[7][342] = settr(1828,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1828,2,207,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1828,2,209,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1828,2,213,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1828,2,255,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1828,2,289,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1828,2,310,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1828,2,314,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1828,2,326,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1828,2,330,0,0,"IF", 1, 2500, 0);
	trans[7][207]	= settr(1693,2,354,212,212,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][208]	= settr(1694,2,354,1,0,"goto :b30", 1, 2500, 0);
	trans[7][343]	= settr(1829,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][209]	= settr(1695,2,344,213,213,"((transition_id==0))", 1, 2500, 0); /* m: 210 -> 344,0 */
	reached7[210] = 1;
	trans[7][210]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][211]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][212]	= settr(1698,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][213]	= settr(1699,2,215,214,214,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][215] = settr(1701,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1701,0,214,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][214]	= settr(1700,2,253,1,0,"(1)", 1, 2500, 0);
	T = trans[7][253] = settr(1739,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1739,2,216,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1739,2,232,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1739,2,250,0,0,"IF", 1, 2500, 0);
	trans[7][216]	= settr(1702,2,220,215,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][220] = settr(1706,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1706,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][217]	= settr(1703,2,219,216,216,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][219] = settr(1705,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1705,0,218,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][218]	= settr(1704,2,224,217,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][224] = settr(1710,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1710,0,221,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][221]	= settr(1707,2,223,218,218,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][223] = settr(1709,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1709,0,222,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][222]	= settr(1708,2,226,219,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 7][226] = settr(1712,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1712,0,225,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][225]	= settr(1711,2,228,220,220,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 7][228] = settr(1714,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1714,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][227]	= settr(1713,2,344,221,221,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 229 -> 344,0 */
	reached7[229] = 1;
	trans[7][229]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][230]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[7][231]	= settr(1717,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][254]	= settr(1740,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][232]	= settr(1718,2,245,222,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][245] = settr(1731,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1731,0,236,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][236] = settr(1722,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1722,0,233,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][233]	= settr(1719,2,235,223,223,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][235] = settr(1721,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1721,0,234,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][234]	= settr(1720,2,240,224,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][240] = settr(1726,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1726,0,237,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][237]	= settr(1723,2,239,225,225,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][239] = settr(1725,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1725,0,238,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][238]	= settr(1724,2,244,226,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][244] = settr(1730,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1730,0,241,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][241]	= settr(1727,2,243,227,227,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][243] = settr(1729,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1729,0,242,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][242]	= settr(1728,2,247,228,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][247] = settr(1733,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1733,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][246]	= settr(1732,2,344,229,229,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 248 -> 344,0 */
	reached7[248] = 1;
	trans[7][248]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][249]	= settr(1735,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][250]	= settr(1736,2,251,2,0,"else", 1, 2500, 0);
	trans[7][251]	= settr(1737,2,344,230,230,"transition_id = -(1)", 1, 2500, 0);
	trans[7][252]	= settr(1738,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][255]	= settr(1741,2,257,231,231,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 7][257] = settr(1743,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1743,0,256,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][256]	= settr(1742,2,287,1,0,"(1)", 1, 2500, 0);
	T = trans[7][287] = settr(1773,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1773,2,258,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1773,2,266,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1773,2,284,0,0,"IF", 1, 2500, 0);
	trans[7][258]	= settr(1744,2,260,232,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][260] = settr(1746,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1746,0,259,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][259]	= settr(1745,2,262,233,233,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 7][262] = settr(1748,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1748,0,261,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][261]	= settr(1747,2,344,234,234,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 263 -> 344,0 */
	reached7[263] = 1;
	trans[7][263]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][264]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[7][265]	= settr(1751,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][288]	= settr(1774,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][266]	= settr(1752,2,279,235,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][279] = settr(1765,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1765,0,270,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][270] = settr(1756,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1756,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][267]	= settr(1753,2,269,236,236,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][269] = settr(1755,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1755,0,268,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][268]	= settr(1754,2,274,237,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][274] = settr(1760,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1760,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][271]	= settr(1757,2,273,238,238,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][273] = settr(1759,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1759,0,272,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][272]	= settr(1758,2,278,239,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][278] = settr(1764,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1764,0,275,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][275]	= settr(1761,2,277,240,240,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][277] = settr(1763,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1763,0,276,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][276]	= settr(1762,2,281,241,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][281] = settr(1767,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1767,0,280,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][280]	= settr(1766,2,344,242,242,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 282 -> 344,0 */
	reached7[282] = 1;
	trans[7][282]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][283]	= settr(1769,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][284]	= settr(1770,2,285,2,0,"else", 1, 2500, 0);
	trans[7][285]	= settr(1771,2,344,243,243,"transition_id = -(1)", 1, 2500, 0);
	trans[7][286]	= settr(1772,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][289]	= settr(1775,2,291,244,244,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 7][291] = settr(1777,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1777,0,290,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][290]	= settr(1776,2,298,1,0,"(1)", 1, 2500, 0);
	T = trans[7][298] = settr(1784,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1784,2,292,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1784,2,295,0,0,"IF", 1, 2500, 0);
	trans[7][292]	= settr(1778,2,294,245,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 7][294] = settr(1780,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1780,0,293,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][293]	= settr(1779,2,306,246,246,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[7][299]	= settr(1785,2,306,1,0,".(goto)", 1, 2500, 0);
	trans[7][295]	= settr(1781,2,297,2,0,"else", 1, 2500, 0);
	T = trans[ 7][297] = settr(1783,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1783,0,296,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][296]	= settr(1782,2,306,247,247,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[7][306] = settr(1792,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1792,2,300,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1792,2,303,0,0,"IF", 1, 2500, 0);
	trans[7][300]	= settr(1786,2,302,248,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 7][302] = settr(1788,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1788,0,301,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][301]	= settr(1787,2,344,249,249,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 308 -> 344,0 */
	reached7[308] = 1;
	trans[7][307]	= settr(1793,2,308,1,0,".(goto)", 1, 2500, 0); /* m: 308 -> 0,344 */
	reached7[308] = 1;
	trans[7][303]	= settr(1789,2,305,2,0,"else", 1, 2500, 0);
	T = trans[ 7][305] = settr(1791,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1791,0,304,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][304]	= settr(1790,2,344,250,250,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 308 -> 344,0 */
	reached7[308] = 1;
	trans[7][308]	= settr(1794,2,344,251,251,"transition_id = -(1)", 1, 2500, 0);
	trans[7][309]	= settr(1795,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][310]	= settr(1796,2,312,252,252,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 7][312] = settr(1798,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1798,0,311,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][311]	= settr(1797,2,324,1,0,"(1)", 1, 2500, 0);
	trans[7][313]	= settr(1799,2,324,1,0,"goto startcheck", 1, 2500, 0);
	trans[7][314]	= settr(1800,2,316,253,253,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 7][316] = settr(1802,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1802,0,315,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][315]	= settr(1801,2,324,1,0,"(1)", 1, 2500, 0);
	T = trans[7][324] = settr(1810,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1810,2,317,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1810,2,320,0,0,"IF", 1, 2500, 0);
	trans[7][317]	= settr(1803,2,344,254,254,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 318 -> 344,0 */
	reached7[318] = 1;
	trans[7][318]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][319]	= settr(1805,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][325]	= settr(1811,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][320]	= settr(1806,2,344,255,255,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 321 -> 344,0 */
	reached7[321] = 1;
	trans[7][321]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][322]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[7][323]	= settr(1809,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][326]	= settr(1812,2,328,256,256,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 7][328] = settr(1814,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1814,0,327,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][327]	= settr(1813,2,340,1,0,"(1)", 1, 2500, 0);
	trans[7][329]	= settr(1815,2,340,1,0,"goto stopcheck", 1, 2500, 0);
	trans[7][330]	= settr(1816,2,332,257,257,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 7][332] = settr(1818,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1818,0,331,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][331]	= settr(1817,2,340,1,0,"(1)", 1, 2500, 0);
	T = trans[7][340] = settr(1826,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1826,2,333,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1826,2,336,0,0,"IF", 1, 2500, 0);
	trans[7][333]	= settr(1819,2,344,258,258,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 334 -> 344,0 */
	reached7[334] = 1;
	trans[7][334]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][335]	= settr(1821,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][341]	= settr(1827,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][336]	= settr(1822,2,344,259,259,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 337 -> 344,0 */
	reached7[337] = 1;
	trans[7][337]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][338]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][339]	= settr(1825,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][344]	= settr(1830,2,345,1,0,"(1)", 1, 2500, 0);
	trans[7][347]	= settr(1833,2,354,1,0,"break", 1, 2500, 0);
	trans[7][349]	= settr(1835,2,350,2,0,"else", 1, 2500, 0);
	trans[7][350]	= settr(1836,2,354,1,0,"(1)", 1, 2500, 0);
	trans[7][354]	= settr(1840,2,465,260,260,"Untitled_msc_lock?_", 1, 2500, 0);
	T = trans[ 7][465] = settr(1951,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1951,0,463,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[7][463] = settr(1949,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1949,2,355,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1949,2,421,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1949,2,461,0,0,"IF", 1, 2500, 0);
	trans[7][355]	= settr(1841,2,381,261,0,"((global_state.untitled_msc.state==0))", 1, 2500, 0);
	T = trans[ 7][381] = settr(1867,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1867,0,356,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][356]	= settr(1842,2,379,262,262,"Untitled_MSC_0_sig0_param0.selection = Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0);
	T = trans[7][379] = settr(1865,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1865,2,357,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1865,2,362,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1865,2,372,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1865,2,377,0,0,"IF", 1, 2500, 0);
	trans[7][357]	= settr(1843,2,361,263,0,"((Untitled_MSC_0_sig0_param0.selection==1))", 1, 2500, 0);
	T = trans[ 7][361] = settr(1847,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1847,0,358,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][358]	= settr(1844,2,360,264,264,"Untitled_MSC_0_sig0_param0.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle", 1, 2500, 0);
	T = trans[ 7][360] = settr(1846,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1846,0,359,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][359]	= settr(1845,2,420,265,0,"assert(((Untitled_MSC_0_sig0_param0.data.goToIdle>=0)&&(Untitled_MSC_0_sig0_param0.data.goToIdle<=0)))", 1, 2500, 0);
	trans[7][380]	= settr(1866,2,420,1,0,".(goto)", 1, 2500, 0);
	trans[7][362]	= settr(1848,2,371,266,0,"((Untitled_MSC_0_sig0_param0.selection==2))", 1, 2500, 0);
	T = trans[ 7][371] = settr(1857,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1857,0,366,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][366] = settr(1852,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1852,0,363,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][363]	= settr(1849,2,365,267,267,"Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][365] = settr(1851,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1851,0,364,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][364]	= settr(1850,2,370,268,0,"assert(((Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1>=0)&&(Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][370] = settr(1856,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1856,0,367,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][367]	= settr(1853,2,369,269,269,"Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][369] = settr(1855,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1855,0,368,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][368]	= settr(1854,2,420,270,0,"assert(((Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2>=0)&&(Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2<=255)))", 1, 2500, 0);
	trans[7][372]	= settr(1858,2,376,271,0,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0);
	T = trans[ 7][376] = settr(1862,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1862,0,373,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][373]	= settr(1859,2,375,272,272,"Untitled_MSC_0_sig0_param0.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport", 1, 2500, 0);
	T = trans[ 7][375] = settr(1861,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1861,0,374,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][374]	= settr(1860,2,420,273,0,"assert(((Untitled_MSC_0_sig0_param0.data.doReport>=0)&&(Untitled_MSC_0_sig0_param0.data.doReport<=0)))", 1, 2500, 0);
	trans[7][377]	= settr(1863,2,378,2,0,"else", 1, 2500, 0);
	trans[7][378]	= settr(1864,2,420,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][420] = settr(1906,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1906,0,382,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][382]	= settr(1868,2,417,274,274,"transition_id = 0", 1, 2500, 0); /* m: 383 -> 0,417 */
	reached7[383] = 1;
	trans[7][383]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[7][418]	= settr(1904,2,417,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][417] = settr(1903,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1903,2,414,0,0,"DO", 1, 2500, 0);
	T = trans[7][414] = settr(1900,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1900,2,384,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1900,2,386,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1900,2,390,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1900,2,402,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1900,2,406,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1900,2,410,0,0,"IF", 1, 2500, 0);
	trans[7][384]	= settr(1870,2,466,275,275,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][385]	= settr(1871,2,466,1,0,"goto :b31", 1, 2500, 0);
	trans[7][415]	= settr(1901,2,416,1,0,".(goto)", 1, 2500, 0);
	trans[7][386]	= settr(1872,2,416,276,276,"((transition_id==0))", 1, 2500, 0); /* m: 387 -> 416,0 */
	reached7[387] = 1;
	trans[7][387]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][388]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][389]	= settr(1875,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][390]	= settr(1876,2,397,277,277,"((transition_id==1))", 1, 2500, 0);
	T = trans[7][397] = settr(1883,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1883,2,391,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1883,2,395,0,0,"IF", 1, 2500, 0);
	trans[7][391]	= settr(1877,2,416,278,278,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 392 -> 416,0 */
	reached7[392] = 1;
	trans[7][392]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][393]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[7][394]	= settr(1880,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][398]	= settr(1884,2,399,1,0,".(goto)", 1, 2500, 0); /* m: 399 -> 0,416 */
	reached7[399] = 1;
	trans[7][395]	= settr(1881,2,396,2,0,"else", 1, 2500, 0);
	trans[7][396]	= settr(1882,2,416,279,279,"(1)", 1, 2500, 0); /* m: 399 -> 416,0 */
	reached7[399] = 1;
	trans[7][399]	= settr(1885,2,416,280,280,"transition_id = -(1)", 1, 2500, 0); /* m: 400 -> 0,416 */
	reached7[400] = 1;
	trans[7][400]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][401]	= settr(1887,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][402]	= settr(1888,2,416,281,281,"((transition_id==2))", 1, 2500, 0); /* m: 403 -> 416,0 */
	reached7[403] = 1;
	trans[7][403]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][404]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][405]	= settr(1891,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][406]	= settr(1892,2,416,282,282,"((transition_id==3))", 1, 2500, 0); /* m: 407 -> 416,0 */
	reached7[407] = 1;
	trans[7][407]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][408]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[7][409]	= settr(1895,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][410]	= settr(1896,2,416,283,283,"((transition_id==4))", 1, 2500, 0); /* m: 411 -> 416,0 */
	reached7[411] = 1;
	trans[7][411]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][412]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[7][413]	= settr(1899,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][416]	= settr(1902,2,417,1,0,"(1)", 1, 2500, 0);
	trans[7][419]	= settr(1905,2,466,1,0,"break", 1, 2500, 0);
	trans[7][464]	= settr(1950,2,466,1,0,".(goto)", 1, 2500, 0);
	trans[7][421]	= settr(1907,2,460,284,0,"((global_state.untitled_msc.state==1))", 1, 2500, 0);
	T = trans[ 7][460] = settr(1946,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1946,0,422,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][422]	= settr(1908,2,457,285,285,"transition_id = 0", 1, 2500, 0); /* m: 423 -> 0,457 */
	reached7[423] = 1;
	trans[7][423]	= settr(0,0,0,0,0,"transition_id = 4",0,0,0);
	trans[7][458]	= settr(1944,2,457,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][457] = settr(1943,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1943,2,454,0,0,"DO", 1, 2500, 0);
	T = trans[7][454] = settr(1940,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1940,2,424,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1940,2,426,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1940,2,430,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1940,2,442,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1940,2,446,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1940,2,450,0,0,"IF", 1, 2500, 0);
	trans[7][424]	= settr(1910,2,466,286,286,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][425]	= settr(1911,2,466,1,0,"goto :b32", 1, 2500, 0);
	trans[7][455]	= settr(1941,2,456,1,0,".(goto)", 1, 2500, 0);
	trans[7][426]	= settr(1912,2,456,287,287,"((transition_id==0))", 1, 2500, 0); /* m: 427 -> 456,0 */
	reached7[427] = 1;
	trans[7][427]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][428]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][429]	= settr(1915,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][430]	= settr(1916,2,437,288,288,"((transition_id==1))", 1, 2500, 0);
	T = trans[7][437] = settr(1923,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1923,2,431,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1923,2,435,0,0,"IF", 1, 2500, 0);
	trans[7][431]	= settr(1917,2,456,289,289,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 432 -> 456,0 */
	reached7[432] = 1;
	trans[7][432]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][433]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[7][434]	= settr(1920,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][438]	= settr(1924,2,439,1,0,".(goto)", 1, 2500, 0); /* m: 439 -> 0,456 */
	reached7[439] = 1;
	trans[7][435]	= settr(1921,2,436,2,0,"else", 1, 2500, 0);
	trans[7][436]	= settr(1922,2,456,290,290,"(1)", 1, 2500, 0); /* m: 439 -> 456,0 */
	reached7[439] = 1;
	trans[7][439]	= settr(1925,2,456,291,291,"transition_id = -(1)", 1, 2500, 0); /* m: 440 -> 0,456 */
	reached7[440] = 1;
	trans[7][440]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][441]	= settr(1927,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][442]	= settr(1928,2,456,292,292,"((transition_id==2))", 1, 2500, 0); /* m: 443 -> 456,0 */
	reached7[443] = 1;
	trans[7][443]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][444]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][445]	= settr(1931,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][446]	= settr(1932,2,456,293,293,"((transition_id==3))", 1, 2500, 0); /* m: 447 -> 456,0 */
	reached7[447] = 1;
	trans[7][447]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][448]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[7][449]	= settr(1935,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][450]	= settr(1936,2,456,294,294,"((transition_id==4))", 1, 2500, 0); /* m: 451 -> 456,0 */
	reached7[451] = 1;
	trans[7][451]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][452]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[7][453]	= settr(1939,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][456]	= settr(1942,2,457,1,0,"(1)", 1, 2500, 0);
	trans[7][459]	= settr(1945,2,466,1,0,"break", 1, 2500, 0);
	trans[7][461]	= settr(1947,2,462,2,0,"else", 1, 2500, 0);
	trans[7][462]	= settr(1948,2,466,1,0,"(1)", 1, 2500, 0);
	trans[7][466]	= settr(1952,2,470,295,295,"Untitled_msc_lock!1", 1, 2500, 0);
	trans[7][467]	= settr(1953,2,470,1,0,"goto Modemanager_systemtc_loop", 1, 2500, 0);
	trans[7][471]	= settr(1957,2,472,1,0,".(goto)", 1, 2500, 0);
	trans[7][468]	= settr(1954,2,469,296,0,"(empty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][469]	= settr(1955,2,472,297,297,"(1)", 1, 2500, 0); /* m: 471 -> 472,0 */
	reached7[471] = 1;
	trans[7][472]	= settr(1958,0,474,298,298,"Modemanager_lock!1", 1, 2500, 0);
	trans[7][476]	= settr(1962,0,477,1,0,"break", 0, 2, 0);
	trans[7][477]	= settr(1963,0,0,299,299,"-end-", 0, 3500, 0);

	/* proctype 6: Modemanager_reading */

	trans[6] = (Trans **) emalloc(172*sizeof(Trans *));

	trans[6][1]	= settr(1316,0,168,300,0,"(inited)", 1, 2, 0);
	trans[6][169]	= settr(1484,0,168,1,0,".(goto)", 0, 2, 0);
	T = trans[6][168] = settr(1483,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1483,0,167,0,0,"DO", 0, 2, 0);
	T = trans[ 6][167] = settr(1482,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1482,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[6][2]	= settr(1317,2,3,301,0,"(nempty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][3]	= settr(1318,2,164,302,302,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[6][164] = settr(1479,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1479,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1479,2,162,0,0,"IF", 1, 2500, 0);
	trans[6][4]	= settr(1319,2,5,303,0,"(nempty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][5]	= settr(1320,2,160,304,304,"Modemanager_reading_channel?Modemanager_reading_signal_parameter", 1, 2500, 0); /* m: 6 -> 160,0 */
	reached6[6] = 1;
	trans[6][6]	= settr(0,0,0,0,0,"Modemanager_reading_channel_used = 1",0,0,0);
	T = trans[ 6][160] = settr(1475,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1475,0,158,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[6][158] = settr(1473,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1473,2,7,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1473,2,156,0,0,"IF", 1, 2500, 0);
	trans[6][7]	= settr(1322,2,11,305,0,"((global_state.modemanager.state==1))", 1, 2500, 0);
	T = trans[ 6][11] = settr(1326,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1326,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][8]	= settr(1323,2,10,306,306,"global_state.modemanager.r = Modemanager_reading_signal_parameter", 1, 2500, 0);
	T = trans[ 6][10] = settr(1325,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1325,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][9]	= settr(1324,2,155,307,0,"assert(((global_state.modemanager.r>=0)&&(global_state.modemanager.r<=255)))", 1, 2500, 0);
	T = trans[ 6][155] = settr(1470,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1470,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][12]	= settr(1327,2,152,308,308,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached6[13] = 1;
	trans[6][13]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[6][153]	= settr(1468,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[6][152] = settr(1467,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1467,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[6][149] = settr(1464,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1464,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1464,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1464,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1464,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1464,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1464,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1464,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1464,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1464,2,137,0,0,"IF", 1, 2500, 0);
	trans[6][14]	= settr(1329,2,164,309,309,"((transition_id==-(1)))", 1, 2500, 0);
	trans[6][15]	= settr(1330,2,164,1,0,"goto :b27", 1, 2500, 0);
	trans[6][150]	= settr(1465,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][16]	= settr(1331,2,151,310,310,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached6[17] = 1;
	trans[6][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[6][19]	= settr(1334,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][20]	= settr(1335,2,22,311,311,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 6][22] = settr(1337,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1337,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][21]	= settr(1336,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[6][60] = settr(1375,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1375,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1375,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1375,2,57,0,0,"IF", 1, 2500, 0);
	trans[6][23]	= settr(1338,2,27,312,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 6][27] = settr(1342,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1342,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][24]	= settr(1339,2,26,313,313,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 6][26] = settr(1341,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1341,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][25]	= settr(1340,2,31,314,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 6][31] = settr(1346,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1346,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][28]	= settr(1343,2,30,315,315,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 6][30] = settr(1345,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1345,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][29]	= settr(1344,2,33,316,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 6][33] = settr(1348,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1348,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][32]	= settr(1347,2,35,317,317,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 6][35] = settr(1350,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1350,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][34]	= settr(1349,2,151,318,318,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached6[36] = 1;
	trans[6][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[6][38]	= settr(1353,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][61]	= settr(1376,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][39]	= settr(1354,2,52,319,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 6][52] = settr(1367,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1367,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 6][43] = settr(1358,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1358,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][40]	= settr(1355,2,42,320,320,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 6][42] = settr(1357,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1357,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][41]	= settr(1356,2,47,321,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 6][47] = settr(1362,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1362,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][44]	= settr(1359,2,46,322,322,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 6][46] = settr(1361,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1361,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][45]	= settr(1360,2,51,323,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 6][51] = settr(1366,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1366,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][48]	= settr(1363,2,50,324,324,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 6][50] = settr(1365,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1365,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][49]	= settr(1364,2,54,325,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 6][54] = settr(1369,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1369,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][53]	= settr(1368,2,151,326,326,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached6[55] = 1;
	trans[6][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][56]	= settr(1371,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][57]	= settr(1372,2,58,2,0,"else", 1, 2500, 0);
	trans[6][58]	= settr(1373,2,151,327,327,"transition_id = -(1)", 1, 2500, 0);
	trans[6][59]	= settr(1374,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][62]	= settr(1377,2,64,328,328,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 6][64] = settr(1379,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1379,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][63]	= settr(1378,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[6][94] = settr(1409,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1409,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1409,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1409,2,91,0,0,"IF", 1, 2500, 0);
	trans[6][65]	= settr(1380,2,67,329,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 6][67] = settr(1382,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1382,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][66]	= settr(1381,2,69,330,330,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 6][69] = settr(1384,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1384,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][68]	= settr(1383,2,151,331,331,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached6[70] = 1;
	trans[6][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[6][72]	= settr(1387,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][95]	= settr(1410,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][73]	= settr(1388,2,86,332,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 6][86] = settr(1401,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1401,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 6][77] = settr(1392,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1392,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][74]	= settr(1389,2,76,333,333,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 6][76] = settr(1391,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1391,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][75]	= settr(1390,2,81,334,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 6][81] = settr(1396,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1396,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][78]	= settr(1393,2,80,335,335,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 6][80] = settr(1395,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1395,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][79]	= settr(1394,2,85,336,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 6][85] = settr(1400,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1400,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][82]	= settr(1397,2,84,337,337,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 6][84] = settr(1399,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1399,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][83]	= settr(1398,2,88,338,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 6][88] = settr(1403,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1403,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][87]	= settr(1402,2,151,339,339,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached6[89] = 1;
	trans[6][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][90]	= settr(1405,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][91]	= settr(1406,2,92,2,0,"else", 1, 2500, 0);
	trans[6][92]	= settr(1407,2,151,340,340,"transition_id = -(1)", 1, 2500, 0);
	trans[6][93]	= settr(1408,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][96]	= settr(1411,2,98,341,341,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 6][98] = settr(1413,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1413,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][97]	= settr(1412,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[6][105] = settr(1420,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1420,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1420,2,102,0,0,"IF", 1, 2500, 0);
	trans[6][99]	= settr(1414,2,101,342,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 6][101] = settr(1416,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1416,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][100]	= settr(1415,2,113,343,343,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[6][106]	= settr(1421,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[6][102]	= settr(1417,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 6][104] = settr(1419,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1419,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][103]	= settr(1418,2,113,344,344,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[6][113] = settr(1428,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1428,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1428,2,110,0,0,"IF", 1, 2500, 0);
	trans[6][107]	= settr(1422,2,109,345,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 6][109] = settr(1424,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1424,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][108]	= settr(1423,2,151,346,346,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached6[115] = 1;
	trans[6][114]	= settr(1429,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached6[115] = 1;
	trans[6][110]	= settr(1425,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 6][112] = settr(1427,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1427,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][111]	= settr(1426,2,151,347,347,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached6[115] = 1;
	trans[6][115]	= settr(1430,2,151,348,348,"transition_id = -(1)", 1, 2500, 0);
	trans[6][116]	= settr(1431,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][117]	= settr(1432,2,119,349,349,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 6][119] = settr(1434,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1434,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][118]	= settr(1433,2,131,1,0,"(1)", 1, 2500, 0);
	trans[6][120]	= settr(1435,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[6][121]	= settr(1436,2,123,350,350,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 6][123] = settr(1438,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1438,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][122]	= settr(1437,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[6][131] = settr(1446,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1446,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1446,2,127,0,0,"IF", 1, 2500, 0);
	trans[6][124]	= settr(1439,2,151,351,351,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached6[125] = 1;
	trans[6][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][126]	= settr(1441,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][132]	= settr(1447,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][127]	= settr(1442,2,151,352,352,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached6[128] = 1;
	trans[6][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[6][130]	= settr(1445,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][133]	= settr(1448,2,135,353,353,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 6][135] = settr(1450,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1450,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][134]	= settr(1449,2,147,1,0,"(1)", 1, 2500, 0);
	trans[6][136]	= settr(1451,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[6][137]	= settr(1452,2,139,354,354,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 6][139] = settr(1454,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1454,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][138]	= settr(1453,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[6][147] = settr(1462,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1462,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1462,2,143,0,0,"IF", 1, 2500, 0);
	trans[6][140]	= settr(1455,2,151,355,355,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached6[141] = 1;
	trans[6][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][142]	= settr(1457,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][148]	= settr(1463,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][143]	= settr(1458,2,151,356,356,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached6[144] = 1;
	trans[6][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[6][146]	= settr(1461,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][151]	= settr(1466,2,152,1,0,"(1)", 1, 2500, 0);
	trans[6][154]	= settr(1469,2,164,1,0,"break", 1, 2500, 0);
	trans[6][159]	= settr(1474,2,164,1,0,".(goto)", 1, 2500, 0);
	trans[6][156]	= settr(1471,2,157,2,0,"else", 1, 2500, 0);
	trans[6][157]	= settr(1472,2,164,1,0,"(1)", 1, 2500, 0);
	trans[6][161]	= settr(1476,2,164,1,0,"goto Modemanager_reading_loop", 1, 2500, 0);
	trans[6][165]	= settr(1480,2,166,1,0,".(goto)", 1, 2500, 0);
	trans[6][162]	= settr(1477,2,163,357,0,"(empty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][163]	= settr(1478,2,166,358,358,"(1)", 1, 2500, 0); /* m: 165 -> 166,0 */
	reached6[165] = 1;
	trans[6][166]	= settr(1481,0,168,359,359,"Modemanager_lock!1", 1, 2500, 0);
	trans[6][170]	= settr(1485,0,171,1,0,"break", 0, 2, 0);
	trans[6][171]	= settr(1486,0,0,360,360,"-end-", 0, 3500, 0);

	/* proctype 5: Modemanager_actuatorTm2 */

	trans[5] = (Trans **) emalloc(321*sizeof(Trans *));

	trans[5][1]	= settr(996,0,317,361,0,"(inited)", 1, 2, 0);
	trans[5][318]	= settr(1313,0,317,1,0,".(goto)", 0, 2, 0);
	T = trans[5][317] = settr(1312,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1312,0,316,0,0,"DO", 0, 2, 0);
	T = trans[ 5][316] = settr(1311,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1311,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[5][2]	= settr(997,2,3,362,0,"(nempty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][3]	= settr(998,2,313,363,363,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[5][313] = settr(1308,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1308,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1308,2,311,0,0,"IF", 1, 2500, 0);
	trans[5][4]	= settr(999,2,5,364,0,"(nempty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][5]	= settr(1000,2,309,365,365,"Modemanager_actuatorTm2_channel?Modemanager_actuatortm2_signal_parameter", 1, 2500, 0); /* m: 6 -> 309,0 */
	reached5[6] = 1;
	trans[5][6]	= settr(0,0,0,0,0,"Modemanager_actuatortm2_channel_used = 1",0,0,0);
	T = trans[ 5][309] = settr(1304,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1304,0,307,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[5][307] = settr(1302,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1302,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1302,2,156,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1302,2,305,0,0,"IF", 1, 2500, 0);
	trans[5][7]	= settr(1002,2,11,366,0,"((global_state.modemanager.state==0))", 1, 2500, 0);
	T = trans[ 5][11] = settr(1006,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1006,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][8]	= settr(1003,2,10,367,367,"global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter", 1, 2500, 0);
	T = trans[ 5][10] = settr(1005,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1005,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][9]	= settr(1004,2,155,368,0,"assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))", 1, 2500, 0);
	T = trans[ 5][155] = settr(1150,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1150,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][12]	= settr(1007,2,152,369,369,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached5[13] = 1;
	trans[5][13]	= settr(0,0,0,0,0,"transition_id = 4",0,0,0);
	trans[5][153]	= settr(1148,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[5][152] = settr(1147,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1147,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[5][149] = settr(1144,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1144,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1144,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1144,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1144,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1144,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1144,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1144,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1144,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1144,2,137,0,0,"IF", 1, 2500, 0);
	trans[5][14]	= settr(1009,2,313,370,370,"((transition_id==-(1)))", 1, 2500, 0);
	trans[5][15]	= settr(1010,2,313,1,0,"goto :b24", 1, 2500, 0);
	trans[5][150]	= settr(1145,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][16]	= settr(1011,2,151,371,371,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached5[17] = 1;
	trans[5][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][19]	= settr(1014,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][20]	= settr(1015,2,22,372,372,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 5][22] = settr(1017,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1017,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][21]	= settr(1016,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[5][60] = settr(1055,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1055,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1055,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1055,2,57,0,0,"IF", 1, 2500, 0);
	trans[5][23]	= settr(1018,2,27,373,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 5][27] = settr(1022,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1022,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][24]	= settr(1019,2,26,374,374,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 5][26] = settr(1021,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1021,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][25]	= settr(1020,2,31,375,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 5][31] = settr(1026,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1026,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][28]	= settr(1023,2,30,376,376,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 5][30] = settr(1025,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1025,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][29]	= settr(1024,2,33,377,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 5][33] = settr(1028,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1028,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][32]	= settr(1027,2,35,378,378,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 5][35] = settr(1030,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1030,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][34]	= settr(1029,2,151,379,379,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached5[36] = 1;
	trans[5][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[5][38]	= settr(1033,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][61]	= settr(1056,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][39]	= settr(1034,2,52,380,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][52] = settr(1047,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1047,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][43] = settr(1038,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1038,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][40]	= settr(1035,2,42,381,381,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][42] = settr(1037,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1037,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][41]	= settr(1036,2,47,382,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][47] = settr(1042,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1042,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][44]	= settr(1039,2,46,383,383,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][46] = settr(1041,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1041,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][45]	= settr(1040,2,51,384,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][51] = settr(1046,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1046,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][48]	= settr(1043,2,50,385,385,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][50] = settr(1045,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1045,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][49]	= settr(1044,2,54,386,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][54] = settr(1049,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1049,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][53]	= settr(1048,2,151,387,387,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached5[55] = 1;
	trans[5][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][56]	= settr(1051,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][57]	= settr(1052,2,58,2,0,"else", 1, 2500, 0);
	trans[5][58]	= settr(1053,2,151,388,388,"transition_id = -(1)", 1, 2500, 0);
	trans[5][59]	= settr(1054,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][62]	= settr(1057,2,64,389,389,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 5][64] = settr(1059,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1059,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][63]	= settr(1058,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[5][94] = settr(1089,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1089,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1089,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1089,2,91,0,0,"IF", 1, 2500, 0);
	trans[5][65]	= settr(1060,2,67,390,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 5][67] = settr(1062,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1062,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][66]	= settr(1061,2,69,391,391,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 5][69] = settr(1064,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1064,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][68]	= settr(1063,2,151,392,392,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached5[70] = 1;
	trans[5][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[5][72]	= settr(1067,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][95]	= settr(1090,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][73]	= settr(1068,2,86,393,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][86] = settr(1081,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1081,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][77] = settr(1072,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1072,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][74]	= settr(1069,2,76,394,394,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][76] = settr(1071,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1071,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][75]	= settr(1070,2,81,395,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][81] = settr(1076,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1076,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][78]	= settr(1073,2,80,396,396,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][80] = settr(1075,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1075,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][79]	= settr(1074,2,85,397,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][85] = settr(1080,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1080,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][82]	= settr(1077,2,84,398,398,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][84] = settr(1079,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1079,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][83]	= settr(1078,2,88,399,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][88] = settr(1083,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1083,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][87]	= settr(1082,2,151,400,400,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached5[89] = 1;
	trans[5][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][90]	= settr(1085,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][91]	= settr(1086,2,92,2,0,"else", 1, 2500, 0);
	trans[5][92]	= settr(1087,2,151,401,401,"transition_id = -(1)", 1, 2500, 0);
	trans[5][93]	= settr(1088,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][96]	= settr(1091,2,98,402,402,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 5][98] = settr(1093,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1093,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][97]	= settr(1092,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[5][105] = settr(1100,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1100,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1100,2,102,0,0,"IF", 1, 2500, 0);
	trans[5][99]	= settr(1094,2,101,403,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 5][101] = settr(1096,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1096,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][100]	= settr(1095,2,113,404,404,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[5][106]	= settr(1101,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[5][102]	= settr(1097,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 5][104] = settr(1099,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1099,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][103]	= settr(1098,2,113,405,405,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[5][113] = settr(1108,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1108,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1108,2,110,0,0,"IF", 1, 2500, 0);
	trans[5][107]	= settr(1102,2,109,406,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 5][109] = settr(1104,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1104,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][108]	= settr(1103,2,151,407,407,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached5[115] = 1;
	trans[5][114]	= settr(1109,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached5[115] = 1;
	trans[5][110]	= settr(1105,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 5][112] = settr(1107,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1107,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][111]	= settr(1106,2,151,408,408,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached5[115] = 1;
	trans[5][115]	= settr(1110,2,151,409,409,"transition_id = -(1)", 1, 2500, 0);
	trans[5][116]	= settr(1111,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][117]	= settr(1112,2,119,410,410,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 5][119] = settr(1114,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1114,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][118]	= settr(1113,2,131,1,0,"(1)", 1, 2500, 0);
	trans[5][120]	= settr(1115,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[5][121]	= settr(1116,2,123,411,411,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 5][123] = settr(1118,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1118,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][122]	= settr(1117,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[5][131] = settr(1126,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1126,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1126,2,127,0,0,"IF", 1, 2500, 0);
	trans[5][124]	= settr(1119,2,151,412,412,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached5[125] = 1;
	trans[5][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][126]	= settr(1121,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][132]	= settr(1127,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][127]	= settr(1122,2,151,413,413,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached5[128] = 1;
	trans[5][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[5][130]	= settr(1125,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][133]	= settr(1128,2,135,414,414,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 5][135] = settr(1130,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1130,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][134]	= settr(1129,2,147,1,0,"(1)", 1, 2500, 0);
	trans[5][136]	= settr(1131,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[5][137]	= settr(1132,2,139,415,415,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 5][139] = settr(1134,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1134,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][138]	= settr(1133,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[5][147] = settr(1142,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1142,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1142,2,143,0,0,"IF", 1, 2500, 0);
	trans[5][140]	= settr(1135,2,151,416,416,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached5[141] = 1;
	trans[5][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][142]	= settr(1137,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][148]	= settr(1143,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][143]	= settr(1138,2,151,417,417,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached5[144] = 1;
	trans[5][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][146]	= settr(1141,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][151]	= settr(1146,2,152,1,0,"(1)", 1, 2500, 0);
	trans[5][154]	= settr(1149,2,313,1,0,"break", 1, 2500, 0);
	trans[5][308]	= settr(1303,2,313,1,0,".(goto)", 1, 2500, 0);
	trans[5][156]	= settr(1151,2,160,418,0,"((global_state.modemanager.state==2))", 1, 2500, 0);
	T = trans[ 5][160] = settr(1155,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1155,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][157]	= settr(1152,2,159,419,419,"global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter", 1, 2500, 0);
	T = trans[ 5][159] = settr(1154,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1154,0,158,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][158]	= settr(1153,2,304,420,0,"assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))", 1, 2500, 0);
	T = trans[ 5][304] = settr(1299,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1299,0,161,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][161]	= settr(1156,2,301,421,421,"transition_id = 0", 1, 2500, 0); /* m: 162 -> 0,301 */
	reached5[162] = 1;
	trans[5][162]	= settr(0,0,0,0,0,"transition_id = 6",0,0,0);
	trans[5][302]	= settr(1297,2,301,1,0,".(goto)", 1, 2500, 0);
	T = trans[5][301] = settr(1296,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1296,2,298,0,0,"DO", 1, 2500, 0);
	T = trans[5][298] = settr(1293,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1293,2,163,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1293,2,165,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1293,2,169,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1293,2,211,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1293,2,245,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1293,2,266,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1293,2,270,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1293,2,282,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1293,2,286,0,0,"IF", 1, 2500, 0);
	trans[5][163]	= settr(1158,2,313,422,422,"((transition_id==-(1)))", 1, 2500, 0);
	trans[5][164]	= settr(1159,2,313,1,0,"goto :b25", 1, 2500, 0);
	trans[5][299]	= settr(1294,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][165]	= settr(1160,2,300,423,423,"((transition_id==0))", 1, 2500, 0); /* m: 166 -> 300,0 */
	reached5[166] = 1;
	trans[5][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][168]	= settr(1163,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][169]	= settr(1164,2,171,424,424,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 5][171] = settr(1166,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1166,0,170,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][170]	= settr(1165,2,209,1,0,"(1)", 1, 2500, 0);
	T = trans[5][209] = settr(1204,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1204,2,172,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1204,2,188,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1204,2,206,0,0,"IF", 1, 2500, 0);
	trans[5][172]	= settr(1167,2,176,425,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 5][176] = settr(1171,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1171,0,173,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][173]	= settr(1168,2,175,426,426,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 5][175] = settr(1170,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1170,0,174,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][174]	= settr(1169,2,180,427,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 5][180] = settr(1175,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1175,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][177]	= settr(1172,2,179,428,428,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 5][179] = settr(1174,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1174,0,178,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][178]	= settr(1173,2,182,429,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 5][182] = settr(1177,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1177,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][181]	= settr(1176,2,184,430,430,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 5][184] = settr(1179,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1179,0,183,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][183]	= settr(1178,2,300,431,431,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 185 -> 300,0 */
	reached5[185] = 1;
	trans[5][185]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][186]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[5][187]	= settr(1182,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][210]	= settr(1205,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][188]	= settr(1183,2,201,432,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][201] = settr(1196,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1196,0,192,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][192] = settr(1187,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1187,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][189]	= settr(1184,2,191,433,433,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][191] = settr(1186,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1186,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][190]	= settr(1185,2,196,434,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][196] = settr(1191,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1191,0,193,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][193]	= settr(1188,2,195,435,435,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][195] = settr(1190,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1190,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][194]	= settr(1189,2,200,436,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][200] = settr(1195,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1195,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][197]	= settr(1192,2,199,437,437,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][199] = settr(1194,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1194,0,198,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][198]	= settr(1193,2,203,438,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][203] = settr(1198,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1198,0,202,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][202]	= settr(1197,2,300,439,439,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 204 -> 300,0 */
	reached5[204] = 1;
	trans[5][204]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][205]	= settr(1200,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][206]	= settr(1201,2,207,2,0,"else", 1, 2500, 0);
	trans[5][207]	= settr(1202,2,300,440,440,"transition_id = -(1)", 1, 2500, 0);
	trans[5][208]	= settr(1203,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][211]	= settr(1206,2,213,441,441,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 5][213] = settr(1208,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1208,0,212,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][212]	= settr(1207,2,243,1,0,"(1)", 1, 2500, 0);
	T = trans[5][243] = settr(1238,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1238,2,214,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1238,2,222,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1238,2,240,0,0,"IF", 1, 2500, 0);
	trans[5][214]	= settr(1209,2,216,442,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 5][216] = settr(1211,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1211,0,215,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][215]	= settr(1210,2,218,443,443,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 5][218] = settr(1213,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1213,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][217]	= settr(1212,2,300,444,444,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 219 -> 300,0 */
	reached5[219] = 1;
	trans[5][219]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][220]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[5][221]	= settr(1216,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][244]	= settr(1239,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][222]	= settr(1217,2,235,445,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][235] = settr(1230,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1230,0,226,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][226] = settr(1221,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1221,0,223,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][223]	= settr(1218,2,225,446,446,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][225] = settr(1220,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1220,0,224,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][224]	= settr(1219,2,230,447,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][230] = settr(1225,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1225,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][227]	= settr(1222,2,229,448,448,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][229] = settr(1224,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1224,0,228,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][228]	= settr(1223,2,234,449,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][234] = settr(1229,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1229,0,231,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][231]	= settr(1226,2,233,450,450,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][233] = settr(1228,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1228,0,232,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][232]	= settr(1227,2,237,451,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][237] = settr(1232,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1232,0,236,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][236]	= settr(1231,2,300,452,452,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 238 -> 300,0 */
	reached5[238] = 1;
	trans[5][238]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][239]	= settr(1234,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][240]	= settr(1235,2,241,2,0,"else", 1, 2500, 0);
	trans[5][241]	= settr(1236,2,300,453,453,"transition_id = -(1)", 1, 2500, 0);
	trans[5][242]	= settr(1237,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][245]	= settr(1240,2,247,454,454,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 5][247] = settr(1242,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1242,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][246]	= settr(1241,2,254,1,0,"(1)", 1, 2500, 0);
	T = trans[5][254] = settr(1249,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1249,2,248,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1249,2,251,0,0,"IF", 1, 2500, 0);
	trans[5][248]	= settr(1243,2,250,455,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 5][250] = settr(1245,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1245,0,249,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][249]	= settr(1244,2,262,456,456,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[5][255]	= settr(1250,2,262,1,0,".(goto)", 1, 2500, 0);
	trans[5][251]	= settr(1246,2,253,2,0,"else", 1, 2500, 0);
	T = trans[ 5][253] = settr(1248,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1248,0,252,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][252]	= settr(1247,2,262,457,457,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[5][262] = settr(1257,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1257,2,256,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1257,2,259,0,0,"IF", 1, 2500, 0);
	trans[5][256]	= settr(1251,2,258,458,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 5][258] = settr(1253,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1253,0,257,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][257]	= settr(1252,2,300,459,459,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached5[264] = 1;
	trans[5][263]	= settr(1258,2,264,1,0,".(goto)", 1, 2500, 0); /* m: 264 -> 0,300 */
	reached5[264] = 1;
	trans[5][259]	= settr(1254,2,261,2,0,"else", 1, 2500, 0);
	T = trans[ 5][261] = settr(1256,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1256,0,260,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][260]	= settr(1255,2,300,460,460,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached5[264] = 1;
	trans[5][264]	= settr(1259,2,300,461,461,"transition_id = -(1)", 1, 2500, 0);
	trans[5][265]	= settr(1260,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][266]	= settr(1261,2,268,462,462,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 5][268] = settr(1263,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1263,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][267]	= settr(1262,2,280,1,0,"(1)", 1, 2500, 0);
	trans[5][269]	= settr(1264,2,280,1,0,"goto startcheck", 1, 2500, 0);
	trans[5][270]	= settr(1265,2,272,463,463,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 5][272] = settr(1267,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1267,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][271]	= settr(1266,2,280,1,0,"(1)", 1, 2500, 0);
	T = trans[5][280] = settr(1275,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1275,2,273,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1275,2,276,0,0,"IF", 1, 2500, 0);
	trans[5][273]	= settr(1268,2,300,464,464,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 274 -> 300,0 */
	reached5[274] = 1;
	trans[5][274]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][275]	= settr(1270,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][281]	= settr(1276,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][276]	= settr(1271,2,300,465,465,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 277 -> 300,0 */
	reached5[277] = 1;
	trans[5][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][278]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[5][279]	= settr(1274,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][282]	= settr(1277,2,284,466,466,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 5][284] = settr(1279,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1279,0,283,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][283]	= settr(1278,2,296,1,0,"(1)", 1, 2500, 0);
	trans[5][285]	= settr(1280,2,296,1,0,"goto stopcheck", 1, 2500, 0);
	trans[5][286]	= settr(1281,2,288,467,467,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 5][288] = settr(1283,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1283,0,287,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][287]	= settr(1282,2,296,1,0,"(1)", 1, 2500, 0);
	T = trans[5][296] = settr(1291,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1291,2,289,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1291,2,292,0,0,"IF", 1, 2500, 0);
	trans[5][289]	= settr(1284,2,300,468,468,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 290 -> 300,0 */
	reached5[290] = 1;
	trans[5][290]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][291]	= settr(1286,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][297]	= settr(1292,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][292]	= settr(1287,2,300,469,469,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 293 -> 300,0 */
	reached5[293] = 1;
	trans[5][293]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][294]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][295]	= settr(1290,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][300]	= settr(1295,2,301,1,0,"(1)", 1, 2500, 0);
	trans[5][303]	= settr(1298,2,313,1,0,"break", 1, 2500, 0);
	trans[5][305]	= settr(1300,2,306,2,0,"else", 1, 2500, 0);
	trans[5][306]	= settr(1301,2,313,1,0,"(1)", 1, 2500, 0);
	trans[5][310]	= settr(1305,2,313,1,0,"goto Modemanager_actuatortm2_loop", 1, 2500, 0);
	trans[5][314]	= settr(1309,2,315,1,0,".(goto)", 1, 2500, 0);
	trans[5][311]	= settr(1306,2,312,470,0,"(empty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][312]	= settr(1307,2,315,471,471,"(1)", 1, 2500, 0); /* m: 314 -> 315,0 */
	reached5[314] = 1;
	trans[5][315]	= settr(1310,0,317,472,472,"Modemanager_lock!1", 1, 2500, 0);
	trans[5][319]	= settr(1314,0,320,1,0,"break", 0, 2, 0);
	trans[5][320]	= settr(1315,0,0,473,473,"-end-", 0, 3500, 0);

	/* proctype 4: Modemanager_actuatorTm1 */

	trans[4] = (Trans **) emalloc(321*sizeof(Trans *));

	trans[4][1]	= settr(676,0,317,474,0,"(inited)", 1, 2, 0);
	trans[4][318]	= settr(993,0,317,1,0,".(goto)", 0, 2, 0);
	T = trans[4][317] = settr(992,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(992,0,316,0,0,"DO", 0, 2, 0);
	T = trans[ 4][316] = settr(991,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(991,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[4][2]	= settr(677,2,3,475,0,"(nempty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][3]	= settr(678,2,313,476,476,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[4][313] = settr(988,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(988,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(988,2,311,0,0,"IF", 1, 2500, 0);
	trans[4][4]	= settr(679,2,5,477,0,"(nempty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][5]	= settr(680,2,309,478,478,"Modemanager_actuatorTm1_channel?Modemanager_actuatortm1_signal_parameter", 1, 2500, 0); /* m: 6 -> 309,0 */
	reached4[6] = 1;
	trans[4][6]	= settr(0,0,0,0,0,"Modemanager_actuatortm1_channel_used = 1",0,0,0);
	T = trans[ 4][309] = settr(984,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(984,0,307,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[4][307] = settr(982,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(982,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(982,2,156,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(982,2,305,0,0,"IF", 1, 2500, 0);
	trans[4][7]	= settr(682,2,11,479,0,"((global_state.modemanager.state==0))", 1, 2500, 0);
	T = trans[ 4][11] = settr(686,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(686,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][8]	= settr(683,2,10,480,480,"global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter", 1, 2500, 0);
	T = trans[ 4][10] = settr(685,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(685,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][9]	= settr(684,2,155,481,0,"assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))", 1, 2500, 0);
	T = trans[ 4][155] = settr(830,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(830,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][12]	= settr(687,2,152,482,482,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached4[13] = 1;
	trans[4][13]	= settr(0,0,0,0,0,"transition_id = 5",0,0,0);
	trans[4][153]	= settr(828,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[4][152] = settr(827,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(827,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[4][149] = settr(824,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(824,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(824,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(824,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(824,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(824,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(824,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(824,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(824,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(824,2,137,0,0,"IF", 1, 2500, 0);
	trans[4][14]	= settr(689,2,313,483,483,"((transition_id==-(1)))", 1, 2500, 0);
	trans[4][15]	= settr(690,2,313,1,0,"goto :b21", 1, 2500, 0);
	trans[4][150]	= settr(825,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][16]	= settr(691,2,151,484,484,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached4[17] = 1;
	trans[4][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][19]	= settr(694,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][20]	= settr(695,2,22,485,485,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 4][22] = settr(697,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(697,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][21]	= settr(696,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[4][60] = settr(735,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(735,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(735,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(735,2,57,0,0,"IF", 1, 2500, 0);
	trans[4][23]	= settr(698,2,27,486,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 4][27] = settr(702,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(702,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][24]	= settr(699,2,26,487,487,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 4][26] = settr(701,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(701,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][25]	= settr(700,2,31,488,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 4][31] = settr(706,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(706,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][28]	= settr(703,2,30,489,489,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 4][30] = settr(705,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(705,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][29]	= settr(704,2,33,490,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 4][33] = settr(708,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(708,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][32]	= settr(707,2,35,491,491,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 4][35] = settr(710,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(710,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][34]	= settr(709,2,151,492,492,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached4[36] = 1;
	trans[4][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[4][38]	= settr(713,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][61]	= settr(736,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][39]	= settr(714,2,52,493,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][52] = settr(727,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(727,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][43] = settr(718,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(718,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][40]	= settr(715,2,42,494,494,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][42] = settr(717,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(717,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][41]	= settr(716,2,47,495,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][47] = settr(722,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(722,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][44]	= settr(719,2,46,496,496,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][46] = settr(721,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(721,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][45]	= settr(720,2,51,497,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][51] = settr(726,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(726,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][48]	= settr(723,2,50,498,498,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][50] = settr(725,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(725,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][49]	= settr(724,2,54,499,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][54] = settr(729,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(729,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][53]	= settr(728,2,151,500,500,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached4[55] = 1;
	trans[4][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][56]	= settr(731,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][57]	= settr(732,2,58,2,0,"else", 1, 2500, 0);
	trans[4][58]	= settr(733,2,151,501,501,"transition_id = -(1)", 1, 2500, 0);
	trans[4][59]	= settr(734,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][62]	= settr(737,2,64,502,502,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 4][64] = settr(739,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(739,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][63]	= settr(738,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[4][94] = settr(769,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(769,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(769,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(769,2,91,0,0,"IF", 1, 2500, 0);
	trans[4][65]	= settr(740,2,67,503,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 4][67] = settr(742,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(742,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][66]	= settr(741,2,69,504,504,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 4][69] = settr(744,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(744,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][68]	= settr(743,2,151,505,505,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached4[70] = 1;
	trans[4][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[4][72]	= settr(747,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][95]	= settr(770,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][73]	= settr(748,2,86,506,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][86] = settr(761,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(761,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][77] = settr(752,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(752,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][74]	= settr(749,2,76,507,507,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][76] = settr(751,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(751,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][75]	= settr(750,2,81,508,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][81] = settr(756,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(756,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][78]	= settr(753,2,80,509,509,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][80] = settr(755,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(755,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][79]	= settr(754,2,85,510,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][85] = settr(760,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(760,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][82]	= settr(757,2,84,511,511,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][84] = settr(759,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(759,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][83]	= settr(758,2,88,512,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][88] = settr(763,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(763,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][87]	= settr(762,2,151,513,513,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached4[89] = 1;
	trans[4][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][90]	= settr(765,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][91]	= settr(766,2,92,2,0,"else", 1, 2500, 0);
	trans[4][92]	= settr(767,2,151,514,514,"transition_id = -(1)", 1, 2500, 0);
	trans[4][93]	= settr(768,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][96]	= settr(771,2,98,515,515,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 4][98] = settr(773,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(773,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][97]	= settr(772,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[4][105] = settr(780,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(780,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(780,2,102,0,0,"IF", 1, 2500, 0);
	trans[4][99]	= settr(774,2,101,516,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 4][101] = settr(776,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(776,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][100]	= settr(775,2,113,517,517,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[4][106]	= settr(781,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[4][102]	= settr(777,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 4][104] = settr(779,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(779,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][103]	= settr(778,2,113,518,518,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[4][113] = settr(788,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(788,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(788,2,110,0,0,"IF", 1, 2500, 0);
	trans[4][107]	= settr(782,2,109,519,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 4][109] = settr(784,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(784,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][108]	= settr(783,2,151,520,520,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached4[115] = 1;
	trans[4][114]	= settr(789,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached4[115] = 1;
	trans[4][110]	= settr(785,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 4][112] = settr(787,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(787,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][111]	= settr(786,2,151,521,521,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached4[115] = 1;
	trans[4][115]	= settr(790,2,151,522,522,"transition_id = -(1)", 1, 2500, 0);
	trans[4][116]	= settr(791,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][117]	= settr(792,2,119,523,523,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 4][119] = settr(794,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(794,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][118]	= settr(793,2,131,1,0,"(1)", 1, 2500, 0);
	trans[4][120]	= settr(795,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[4][121]	= settr(796,2,123,524,524,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 4][123] = settr(798,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(798,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][122]	= settr(797,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[4][131] = settr(806,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(806,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(806,2,127,0,0,"IF", 1, 2500, 0);
	trans[4][124]	= settr(799,2,151,525,525,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached4[125] = 1;
	trans[4][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][126]	= settr(801,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][132]	= settr(807,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][127]	= settr(802,2,151,526,526,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached4[128] = 1;
	trans[4][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[4][130]	= settr(805,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][133]	= settr(808,2,135,527,527,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 4][135] = settr(810,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(810,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][134]	= settr(809,2,147,1,0,"(1)", 1, 2500, 0);
	trans[4][136]	= settr(811,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[4][137]	= settr(812,2,139,528,528,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 4][139] = settr(814,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(814,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][138]	= settr(813,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[4][147] = settr(822,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(822,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(822,2,143,0,0,"IF", 1, 2500, 0);
	trans[4][140]	= settr(815,2,151,529,529,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached4[141] = 1;
	trans[4][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][142]	= settr(817,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][148]	= settr(823,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][143]	= settr(818,2,151,530,530,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached4[144] = 1;
	trans[4][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][146]	= settr(821,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][151]	= settr(826,2,152,1,0,"(1)", 1, 2500, 0);
	trans[4][154]	= settr(829,2,313,1,0,"break", 1, 2500, 0);
	trans[4][308]	= settr(983,2,313,1,0,".(goto)", 1, 2500, 0);
	trans[4][156]	= settr(831,2,160,531,0,"((global_state.modemanager.state==2))", 1, 2500, 0);
	T = trans[ 4][160] = settr(835,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(835,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][157]	= settr(832,2,159,532,532,"global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter", 1, 2500, 0);
	T = trans[ 4][159] = settr(834,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(834,0,158,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][158]	= settr(833,2,304,533,0,"assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))", 1, 2500, 0);
	T = trans[ 4][304] = settr(979,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(979,0,161,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][161]	= settr(836,2,301,534,534,"transition_id = 0", 1, 2500, 0); /* m: 162 -> 0,301 */
	reached4[162] = 1;
	trans[4][162]	= settr(0,0,0,0,0,"transition_id = 7",0,0,0);
	trans[4][302]	= settr(977,2,301,1,0,".(goto)", 1, 2500, 0);
	T = trans[4][301] = settr(976,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(976,2,298,0,0,"DO", 1, 2500, 0);
	T = trans[4][298] = settr(973,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(973,2,163,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(973,2,165,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(973,2,169,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(973,2,211,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(973,2,245,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(973,2,266,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(973,2,270,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(973,2,282,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(973,2,286,0,0,"IF", 1, 2500, 0);
	trans[4][163]	= settr(838,2,313,535,535,"((transition_id==-(1)))", 1, 2500, 0);
	trans[4][164]	= settr(839,2,313,1,0,"goto :b22", 1, 2500, 0);
	trans[4][299]	= settr(974,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][165]	= settr(840,2,300,536,536,"((transition_id==0))", 1, 2500, 0); /* m: 166 -> 300,0 */
	reached4[166] = 1;
	trans[4][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][168]	= settr(843,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][169]	= settr(844,2,171,537,537,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 4][171] = settr(846,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(846,0,170,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][170]	= settr(845,2,209,1,0,"(1)", 1, 2500, 0);
	T = trans[4][209] = settr(884,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(884,2,172,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(884,2,188,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(884,2,206,0,0,"IF", 1, 2500, 0);
	trans[4][172]	= settr(847,2,176,538,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 4][176] = settr(851,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(851,0,173,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][173]	= settr(848,2,175,539,539,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 4][175] = settr(850,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(850,0,174,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][174]	= settr(849,2,180,540,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 4][180] = settr(855,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(855,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][177]	= settr(852,2,179,541,541,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 4][179] = settr(854,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(854,0,178,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][178]	= settr(853,2,182,542,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 4][182] = settr(857,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(857,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][181]	= settr(856,2,184,543,543,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 4][184] = settr(859,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(859,0,183,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][183]	= settr(858,2,300,544,544,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 185 -> 300,0 */
	reached4[185] = 1;
	trans[4][185]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][186]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[4][187]	= settr(862,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][210]	= settr(885,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][188]	= settr(863,2,201,545,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][201] = settr(876,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(876,0,192,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][192] = settr(867,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(867,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][189]	= settr(864,2,191,546,546,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][191] = settr(866,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(866,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][190]	= settr(865,2,196,547,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][196] = settr(871,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(871,0,193,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][193]	= settr(868,2,195,548,548,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][195] = settr(870,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(870,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][194]	= settr(869,2,200,549,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][200] = settr(875,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(875,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][197]	= settr(872,2,199,550,550,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][199] = settr(874,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(874,0,198,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][198]	= settr(873,2,203,551,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][203] = settr(878,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(878,0,202,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][202]	= settr(877,2,300,552,552,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 204 -> 300,0 */
	reached4[204] = 1;
	trans[4][204]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][205]	= settr(880,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][206]	= settr(881,2,207,2,0,"else", 1, 2500, 0);
	trans[4][207]	= settr(882,2,300,553,553,"transition_id = -(1)", 1, 2500, 0);
	trans[4][208]	= settr(883,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][211]	= settr(886,2,213,554,554,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 4][213] = settr(888,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(888,0,212,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][212]	= settr(887,2,243,1,0,"(1)", 1, 2500, 0);
	T = trans[4][243] = settr(918,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(918,2,214,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(918,2,222,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(918,2,240,0,0,"IF", 1, 2500, 0);
	trans[4][214]	= settr(889,2,216,555,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 4][216] = settr(891,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(891,0,215,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][215]	= settr(890,2,218,556,556,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 4][218] = settr(893,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(893,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][217]	= settr(892,2,300,557,557,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 219 -> 300,0 */
	reached4[219] = 1;
	trans[4][219]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][220]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[4][221]	= settr(896,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][244]	= settr(919,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][222]	= settr(897,2,235,558,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][235] = settr(910,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(910,0,226,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][226] = settr(901,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(901,0,223,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][223]	= settr(898,2,225,559,559,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][225] = settr(900,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(900,0,224,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][224]	= settr(899,2,230,560,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][230] = settr(905,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(905,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][227]	= settr(902,2,229,561,561,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][229] = settr(904,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(904,0,228,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][228]	= settr(903,2,234,562,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][234] = settr(909,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(909,0,231,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][231]	= settr(906,2,233,563,563,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][233] = settr(908,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(908,0,232,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][232]	= settr(907,2,237,564,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][237] = settr(912,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(912,0,236,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][236]	= settr(911,2,300,565,565,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 238 -> 300,0 */
	reached4[238] = 1;
	trans[4][238]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][239]	= settr(914,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][240]	= settr(915,2,241,2,0,"else", 1, 2500, 0);
	trans[4][241]	= settr(916,2,300,566,566,"transition_id = -(1)", 1, 2500, 0);
	trans[4][242]	= settr(917,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][245]	= settr(920,2,247,567,567,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 4][247] = settr(922,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(922,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][246]	= settr(921,2,254,1,0,"(1)", 1, 2500, 0);
	T = trans[4][254] = settr(929,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(929,2,248,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(929,2,251,0,0,"IF", 1, 2500, 0);
	trans[4][248]	= settr(923,2,250,568,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 4][250] = settr(925,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(925,0,249,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][249]	= settr(924,2,262,569,569,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[4][255]	= settr(930,2,262,1,0,".(goto)", 1, 2500, 0);
	trans[4][251]	= settr(926,2,253,2,0,"else", 1, 2500, 0);
	T = trans[ 4][253] = settr(928,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(928,0,252,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][252]	= settr(927,2,262,570,570,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[4][262] = settr(937,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(937,2,256,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(937,2,259,0,0,"IF", 1, 2500, 0);
	trans[4][256]	= settr(931,2,258,571,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 4][258] = settr(933,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(933,0,257,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][257]	= settr(932,2,300,572,572,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached4[264] = 1;
	trans[4][263]	= settr(938,2,264,1,0,".(goto)", 1, 2500, 0); /* m: 264 -> 0,300 */
	reached4[264] = 1;
	trans[4][259]	= settr(934,2,261,2,0,"else", 1, 2500, 0);
	T = trans[ 4][261] = settr(936,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(936,0,260,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][260]	= settr(935,2,300,573,573,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached4[264] = 1;
	trans[4][264]	= settr(939,2,300,574,574,"transition_id = -(1)", 1, 2500, 0);
	trans[4][265]	= settr(940,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][266]	= settr(941,2,268,575,575,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 4][268] = settr(943,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(943,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][267]	= settr(942,2,280,1,0,"(1)", 1, 2500, 0);
	trans[4][269]	= settr(944,2,280,1,0,"goto startcheck", 1, 2500, 0);
	trans[4][270]	= settr(945,2,272,576,576,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 4][272] = settr(947,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(947,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][271]	= settr(946,2,280,1,0,"(1)", 1, 2500, 0);
	T = trans[4][280] = settr(955,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(955,2,273,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(955,2,276,0,0,"IF", 1, 2500, 0);
	trans[4][273]	= settr(948,2,300,577,577,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 274 -> 300,0 */
	reached4[274] = 1;
	trans[4][274]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][275]	= settr(950,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][281]	= settr(956,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][276]	= settr(951,2,300,578,578,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 277 -> 300,0 */
	reached4[277] = 1;
	trans[4][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][278]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[4][279]	= settr(954,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][282]	= settr(957,2,284,579,579,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 4][284] = settr(959,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(959,0,283,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][283]	= settr(958,2,296,1,0,"(1)", 1, 2500, 0);
	trans[4][285]	= settr(960,2,296,1,0,"goto stopcheck", 1, 2500, 0);
	trans[4][286]	= settr(961,2,288,580,580,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 4][288] = settr(963,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(963,0,287,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][287]	= settr(962,2,296,1,0,"(1)", 1, 2500, 0);
	T = trans[4][296] = settr(971,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(971,2,289,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(971,2,292,0,0,"IF", 1, 2500, 0);
	trans[4][289]	= settr(964,2,300,581,581,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 290 -> 300,0 */
	reached4[290] = 1;
	trans[4][290]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][291]	= settr(966,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][297]	= settr(972,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][292]	= settr(967,2,300,582,582,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 293 -> 300,0 */
	reached4[293] = 1;
	trans[4][293]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][294]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][295]	= settr(970,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][300]	= settr(975,2,301,1,0,"(1)", 1, 2500, 0);
	trans[4][303]	= settr(978,2,313,1,0,"break", 1, 2500, 0);
	trans[4][305]	= settr(980,2,306,2,0,"else", 1, 2500, 0);
	trans[4][306]	= settr(981,2,313,1,0,"(1)", 1, 2500, 0);
	trans[4][310]	= settr(985,2,313,1,0,"goto Modemanager_actuatortm1_loop", 1, 2500, 0);
	trans[4][314]	= settr(989,2,315,1,0,".(goto)", 1, 2500, 0);
	trans[4][311]	= settr(986,2,312,583,0,"(empty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][312]	= settr(987,2,315,584,584,"(1)", 1, 2500, 0); /* m: 314 -> 315,0 */
	reached4[314] = 1;
	trans[4][315]	= settr(990,0,317,585,585,"Modemanager_lock!1", 1, 2500, 0);
	trans[4][319]	= settr(994,0,320,1,0,"break", 0, 2, 0);
	trans[4][320]	= settr(995,0,0,586,586,"-end-", 0, 3500, 0);

	/* proctype 3: Egse_systemTc */

	trans[3] = (Trans **) emalloc(110*sizeof(Trans *));

	trans[3][1]	= settr(567,0,2,587,0,"(inited)", 1, 2, 0);
	trans[3][2]	= settr(568,0,106,588,588,"", 0, 2, 0); /* m: 3 -> 0,106 */
	reached3[3] = 1;
	trans[3][3]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[3][4]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[3][107]	= settr(673,0,106,1,0,".(goto)", 0, 2, 0);
	T = trans[3][106] = settr(672,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(672,0,5,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(672,0,104,0,0,"DO", 0, 2, 0);
	trans[3][5]	= settr(571,0,102,589,0,"((inputVectorCounter<=1))", 0, 2, 0);
	T = trans[ 3][102] = settr(668,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(668,2,99,0,0,"ATOMIC", 1, 2, 0);
	T = trans[ 3][99] = settr(665,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(665,0,97,0,0,"sub-sequence", 1, 2, 0);
	T = trans[3][97] = settr(663,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(663,2,6,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(663,2,21,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(663,2,82,0,0,"IF", 1, 2, 0);
	trans[3][6]	= settr(572,2,19,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][19] = settr(585,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(585,0,7,0,0,"sub-sequence", 1, 2, 0);
	trans[3][7]	= settr(573,2,16,590,590,"Dummy_tmp = 0", 1, 2, 0);
	T = trans[3][16] = settr(582,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(582,2,15,0,0,"IF", 1, 2, 0);
	T = trans[ 3][15] = settr(581,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(581,0,8,0,0,"sub-sequence", 1, 2, 0);
	trans[3][8]	= settr(574,2,12,591,591,"Dummy_tmp = 0", 1, 2, 0);
	trans[3][13]	= settr(579,2,12,1,0,".(goto)", 1, 2, 0);
	T = trans[3][12] = settr(578,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(578,2,11,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(578,2,9,0,0,"DO", 1, 2, 0);
	trans[3][11]	= settr(577,2,101,592,592,"goto :b12", 1, 2, 0); /* m: 18 -> 0,101 */
	reached3[18] = 1;
	trans[3][9]	= settr(575,2,12,593,593,"((Dummy_tmp<0))", 1, 2, 0); /* m: 10 -> 12,0 */
	reached3[10] = 1;
	trans[3][10]	= settr(0,0,0,0,0,"Dummy_tmp = (Dummy_tmp+1)",0,0,0);
	trans[3][14]	= settr(580,2,18,1,0,"break", 1, 2, 0);
	trans[3][17]	= settr(583,2,18,1,0,".(goto)", 1, 2, 0); /* m: 18 -> 0,101 */
	reached3[18] = 1;
	trans[3][18]	= settr(584,2,101,594,594,"value.data.goToIdle = Dummy_tmp", 1, 2, 0); /* m: 20 -> 0,101 */
	reached3[20] = 1;
	trans[3][20]	= settr(0,0,0,0,0,"value.selection = 1",0,0,0);
	trans[3][98]	= settr(664,2,101,1,0,".(goto)", 1, 2, 0);
	trans[3][21]	= settr(587,2,80,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][80] = settr(646,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(646,0,50,0,0,"sub-sequence", 1, 2, 0);
	T = trans[ 3][50] = settr(616,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(616,0,22,0,0,"sub-sequence", 1, 2, 0);
	trans[3][22]	= settr(588,2,47,595,595,"OptimizedDataItem_tmp = 0", 1, 2, 0);
	T = trans[3][47] = settr(613,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(613,2,30,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(613,2,38,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(613,2,46,0,0,"IF", 1, 2, 0);
	T = trans[ 3][30] = settr(596,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(596,0,23,0,0,"sub-sequence", 1, 2, 0);
	trans[3][23]	= settr(589,2,27,596,596,"OptimizedDataItem_tmp = 10", 1, 2, 0);
	trans[3][28]	= settr(594,2,27,1,0,".(goto)", 1, 2, 0);
	T = trans[3][27] = settr(593,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(593,2,26,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(593,2,24,0,0,"DO", 1, 2, 0);
	trans[3][26]	= settr(592,2,79,597,597,"goto :b13", 1, 2, 0); /* m: 49 -> 0,79 */
	reached3[49] = 1;
	trans[3][24]	= settr(590,2,27,598,598,"((OptimizedDataItem_tmp<10))", 1, 2, 0); /* m: 25 -> 27,0 */
	reached3[25] = 1;
	trans[3][25]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][29]	= settr(595,2,49,1,0,"break", 1, 2, 0);
	trans[3][48]	= settr(614,2,49,1,0,".(goto)", 1, 2, 0); /* m: 49 -> 0,79 */
	reached3[49] = 1;
	T = trans[ 3][38] = settr(604,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(604,0,31,0,0,"sub-sequence", 1, 2, 0);
	trans[3][31]	= settr(597,2,35,599,599,"OptimizedDataItem_tmp = 50", 1, 2, 0);
	trans[3][36]	= settr(602,2,35,1,0,".(goto)", 1, 2, 0);
	T = trans[3][35] = settr(601,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(601,2,34,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(601,2,32,0,0,"DO", 1, 2, 0);
	trans[3][34]	= settr(600,2,79,600,600,"goto :b14", 1, 2, 0); /* m: 49 -> 0,79 */
	reached3[49] = 1;
	trans[3][32]	= settr(598,2,35,601,601,"((OptimizedDataItem_tmp<50))", 1, 2, 0); /* m: 33 -> 35,0 */
	reached3[33] = 1;
	trans[3][33]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][37]	= settr(603,2,49,1,0,"break", 1, 2, 0);
	T = trans[ 3][46] = settr(612,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(612,0,39,0,0,"sub-sequence", 1, 2, 0);
	trans[3][39]	= settr(605,2,43,602,602,"OptimizedDataItem_tmp = 100", 1, 2, 0);
	trans[3][44]	= settr(610,2,43,1,0,".(goto)", 1, 2, 0);
	T = trans[3][43] = settr(609,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(609,2,42,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(609,2,40,0,0,"DO", 1, 2, 0);
	trans[3][42]	= settr(608,2,79,603,603,"goto :b15", 1, 2, 0); /* m: 49 -> 0,79 */
	reached3[49] = 1;
	trans[3][40]	= settr(606,2,43,604,604,"((OptimizedDataItem_tmp<100))", 1, 2, 0); /* m: 41 -> 43,0 */
	reached3[41] = 1;
	trans[3][41]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][45]	= settr(611,2,49,1,0,"break", 1, 2, 0);
	trans[3][49]	= settr(615,2,79,605,605,"value.data.goToOperational.threshold1 = OptimizedDataItem_tmp", 1, 2, 0);
	T = trans[ 3][79] = settr(645,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(645,0,51,0,0,"sub-sequence", 1, 2, 0);
	trans[3][51]	= settr(617,2,76,606,606,"OptimizedDataItem_tmp = 0", 1, 2, 0);
	T = trans[3][76] = settr(642,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(642,2,59,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(642,2,67,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(642,2,75,0,0,"IF", 1, 2, 0);
	T = trans[ 3][59] = settr(625,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(625,0,52,0,0,"sub-sequence", 1, 2, 0);
	trans[3][52]	= settr(618,2,56,607,607,"OptimizedDataItem_tmp = 10", 1, 2, 0);
	trans[3][57]	= settr(623,2,56,1,0,".(goto)", 1, 2, 0);
	T = trans[3][56] = settr(622,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(622,2,55,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(622,2,53,0,0,"DO", 1, 2, 0);
	trans[3][55]	= settr(621,2,101,608,608,"goto :b16", 1, 2, 0); /* m: 78 -> 0,101 */
	reached3[78] = 1;
	trans[3][53]	= settr(619,2,56,609,609,"((OptimizedDataItem_tmp<10))", 1, 2, 0); /* m: 54 -> 56,0 */
	reached3[54] = 1;
	trans[3][54]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][58]	= settr(624,2,78,1,0,"break", 1, 2, 0);
	trans[3][77]	= settr(643,2,78,1,0,".(goto)", 1, 2, 0); /* m: 78 -> 0,101 */
	reached3[78] = 1;
	T = trans[ 3][67] = settr(633,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(633,0,60,0,0,"sub-sequence", 1, 2, 0);
	trans[3][60]	= settr(626,2,64,610,610,"OptimizedDataItem_tmp = 50", 1, 2, 0);
	trans[3][65]	= settr(631,2,64,1,0,".(goto)", 1, 2, 0);
	T = trans[3][64] = settr(630,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(630,2,63,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(630,2,61,0,0,"DO", 1, 2, 0);
	trans[3][63]	= settr(629,2,101,611,611,"goto :b17", 1, 2, 0); /* m: 78 -> 0,101 */
	reached3[78] = 1;
	trans[3][61]	= settr(627,2,64,612,612,"((OptimizedDataItem_tmp<50))", 1, 2, 0); /* m: 62 -> 64,0 */
	reached3[62] = 1;
	trans[3][62]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][66]	= settr(632,2,78,1,0,"break", 1, 2, 0);
	T = trans[ 3][75] = settr(641,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(641,0,68,0,0,"sub-sequence", 1, 2, 0);
	trans[3][68]	= settr(634,2,72,613,613,"OptimizedDataItem_tmp = 100", 1, 2, 0);
	trans[3][73]	= settr(639,2,72,1,0,".(goto)", 1, 2, 0);
	T = trans[3][72] = settr(638,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(638,2,71,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(638,2,69,0,0,"DO", 1, 2, 0);
	trans[3][71]	= settr(637,2,101,614,614,"goto :b18", 1, 2, 0); /* m: 78 -> 0,101 */
	reached3[78] = 1;
	trans[3][69]	= settr(635,2,72,615,615,"((OptimizedDataItem_tmp<100))", 1, 2, 0); /* m: 70 -> 72,0 */
	reached3[70] = 1;
	trans[3][70]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][74]	= settr(640,2,78,1,0,"break", 1, 2, 0);
	trans[3][78]	= settr(644,2,101,616,616,"value.data.goToOperational.threshold2 = OptimizedDataItem_tmp", 1, 2, 0); /* m: 81 -> 0,101 */
	reached3[81] = 1;
	trans[3][81]	= settr(0,0,0,0,0,"value.selection = 2",0,0,0);
	trans[3][82]	= settr(648,2,95,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][95] = settr(661,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(661,0,83,0,0,"sub-sequence", 1, 2, 0);
	trans[3][83]	= settr(649,2,92,617,617,"Dummy_tmp = 0", 1, 2, 0);
	T = trans[3][92] = settr(658,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(658,2,91,0,0,"IF", 1, 2, 0);
	T = trans[ 3][91] = settr(657,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(657,0,84,0,0,"sub-sequence", 1, 2, 0);
	trans[3][84]	= settr(650,2,88,618,618,"Dummy_tmp = 0", 1, 2, 0);
	trans[3][89]	= settr(655,2,88,1,0,".(goto)", 1, 2, 0);
	T = trans[3][88] = settr(654,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(654,2,87,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(654,2,85,0,0,"DO", 1, 2, 0);
	trans[3][87]	= settr(653,2,101,619,619,"goto :b19", 1, 2, 0); /* m: 94 -> 0,101 */
	reached3[94] = 1;
	trans[3][85]	= settr(651,2,88,620,620,"((Dummy_tmp<0))", 1, 2, 0); /* m: 86 -> 88,0 */
	reached3[86] = 1;
	trans[3][86]	= settr(0,0,0,0,0,"Dummy_tmp = (Dummy_tmp+1)",0,0,0);
	trans[3][90]	= settr(656,2,94,1,0,"break", 1, 2, 0);
	trans[3][93]	= settr(659,2,94,1,0,".(goto)", 1, 2, 0); /* m: 94 -> 0,101 */
	reached3[94] = 1;
	trans[3][94]	= settr(660,2,101,621,621,"value.data.doReport = Dummy_tmp", 1, 2, 0); /* m: 96 -> 0,101 */
	reached3[96] = 1;
	trans[3][96]	= settr(0,0,0,0,0,"value.selection = 3",0,0,0);
	T = trans[ 3][101] = settr(667,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(667,0,100,0,0,"sub-sequence", 1, 2, 0);
	trans[3][100]	= settr(666,0,106,622,622,"Modemanager_systemTc_channel!value.data.goToIdle,value.data.goToOperational.threshold1,value.data.goToOperational.threshold2,value.data.doReport,value.selection", 1, 9, 0); /* m: 103 -> 106,0 */
	reached3[103] = 1;
	trans[3][103]	= settr(0,0,0,0,0,"inputVectorCounter = (inputVectorCounter+1)",0,0,0);
	trans[3][104]	= settr(670,0,109,2,0,"else", 0, 2, 0);
	trans[3][105]	= settr(671,0,109,1,0,"goto :b11", 0, 2, 0);
	trans[3][108]	= settr(674,0,109,1,0,"break", 0, 2, 0);
	trans[3][109]	= settr(675,0,0,623,623,"-end-", 0, 3500, 0);

	/* proctype 2: Egse_systemTm */

	trans[2] = (Trans **) emalloc(116*sizeof(Trans *));

	trans[2][1]	= settr(452,0,112,624,0,"(inited)", 1, 2, 0);
	trans[2][113]	= settr(564,0,112,1,0,".(goto)", 0, 2, 0);
	T = trans[2][112] = settr(563,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(563,0,111,0,0,"DO", 0, 2, 0);
	T = trans[ 2][111] = settr(562,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(562,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[2][2]	= settr(453,2,109,625,0,"(nempty(Egse_systemTm_channel))", 1, 2500, 0);
	T = trans[2][109] = settr(560,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(560,2,3,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(560,2,107,0,0,"IF", 1, 2500, 0);
	trans[2][3]	= settr(454,2,4,626,0,"(nempty(Egse_systemTm_channel))", 1, 2500, 0);
	trans[2][4]	= settr(455,2,6,627,627,"Egse_systemTm_channel?Egse_systemtm_signal_parameter.reading,Egse_systemtm_signal_parameter.status1,Egse_systemtm_signal_parameter.status2", 1, 2500, 0); /* m: 5 -> 6,0 */
	reached2[5] = 1;
	trans[2][5]	= settr(0,0,0,0,0,"Egse_systemtm_channel_used = 1",0,0,0);
	trans[2][6]	= settr(457,2,104,628,628,"Untitled_msc_lock?_", 1, 2500, 0);
	T = trans[ 2][104] = settr(555,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(555,0,102,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[2][102] = settr(553,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(553,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(553,2,47,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(553,2,100,0,0,"IF", 1, 2500, 0);
	trans[2][7]	= settr(458,2,46,629,0,"((global_state.untitled_msc.state==0))", 1, 2500, 0);
	T = trans[ 2][46] = settr(497,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(497,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][8]	= settr(459,2,43,630,630,"transition_id = 0", 1, 2500, 0); /* m: 9 -> 0,43 */
	reached2[9] = 1;
	trans[2][9]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[2][44]	= settr(495,2,43,1,0,".(goto)", 1, 2500, 0);
	T = trans[2][43] = settr(494,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(494,2,40,0,0,"DO", 1, 2500, 0);
	T = trans[2][40] = settr(491,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(491,2,10,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(491,2,12,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(491,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(491,2,28,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(491,2,32,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(491,2,36,0,0,"IF", 1, 2500, 0);
	trans[2][10]	= settr(461,2,105,631,631,"((transition_id==-(1)))", 1, 2500, 0);
	trans[2][11]	= settr(462,2,105,1,0,"goto :b9", 1, 2500, 0);
	trans[2][41]	= settr(492,2,42,1,0,".(goto)", 1, 2500, 0);
	trans[2][12]	= settr(463,2,42,632,632,"((transition_id==0))", 1, 2500, 0); /* m: 13 -> 42,0 */
	reached2[13] = 1;
	trans[2][13]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][14]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][15]	= settr(466,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][16]	= settr(467,2,23,633,633,"((transition_id==1))", 1, 2500, 0);
	T = trans[2][23] = settr(474,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(474,2,17,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(474,2,21,0,0,"IF", 1, 2500, 0);
	trans[2][17]	= settr(468,2,42,634,634,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 18 -> 42,0 */
	reached2[18] = 1;
	trans[2][18]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][19]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[2][20]	= settr(471,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][24]	= settr(475,2,25,1,0,".(goto)", 1, 2500, 0); /* m: 25 -> 0,42 */
	reached2[25] = 1;
	trans[2][21]	= settr(472,2,22,2,0,"else", 1, 2500, 0);
	trans[2][22]	= settr(473,2,42,635,635,"(1)", 1, 2500, 0); /* m: 25 -> 42,0 */
	reached2[25] = 1;
	trans[2][25]	= settr(476,2,42,636,636,"transition_id = -(1)", 1, 2500, 0); /* m: 26 -> 0,42 */
	reached2[26] = 1;
	trans[2][26]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][27]	= settr(478,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][28]	= settr(479,2,42,637,637,"((transition_id==2))", 1, 2500, 0); /* m: 29 -> 42,0 */
	reached2[29] = 1;
	trans[2][29]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][30]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][31]	= settr(482,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][32]	= settr(483,2,42,638,638,"((transition_id==3))", 1, 2500, 0); /* m: 33 -> 42,0 */
	reached2[33] = 1;
	trans[2][33]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][34]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[2][35]	= settr(486,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][36]	= settr(487,2,42,639,639,"((transition_id==4))", 1, 2500, 0); /* m: 37 -> 42,0 */
	reached2[37] = 1;
	trans[2][37]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][38]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[2][39]	= settr(490,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][42]	= settr(493,2,43,1,0,"(1)", 1, 2500, 0);
	trans[2][45]	= settr(496,2,105,1,0,"break", 1, 2500, 0);
	trans[2][103]	= settr(554,2,105,1,0,".(goto)", 1, 2500, 0);
	trans[2][47]	= settr(498,2,60,640,0,"((global_state.untitled_msc.state==1))", 1, 2500, 0);
	T = trans[ 2][60] = settr(511,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(511,0,51,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 2][51] = settr(502,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(502,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][48]	= settr(499,2,50,641,641,"Untitled_MSC_0_sig1_param0.reading = Egse_systemtm_signal_parameter.reading", 1, 2500, 0);
	T = trans[ 2][50] = settr(501,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(501,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][49]	= settr(500,2,55,642,0,"assert(((Untitled_MSC_0_sig1_param0.reading>=0)&&(Untitled_MSC_0_sig1_param0.reading<=255)))", 1, 2500, 0);
	T = trans[ 2][55] = settr(506,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(506,0,52,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][52]	= settr(503,2,54,643,643,"Untitled_MSC_0_sig1_param0.status1 = Egse_systemtm_signal_parameter.status1", 1, 2500, 0);
	T = trans[ 2][54] = settr(505,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(505,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][53]	= settr(504,2,59,644,0,"assert((((Untitled_MSC_0_sig1_param0.status1==0)||(Untitled_MSC_0_sig1_param0.status1==1))||(Untitled_MSC_0_sig1_param0.status1==2)))", 1, 2500, 0);
	T = trans[ 2][59] = settr(510,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(510,0,56,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][56]	= settr(507,2,58,645,645,"Untitled_MSC_0_sig1_param0.status2 = Egse_systemtm_signal_parameter.status2", 1, 2500, 0);
	T = trans[ 2][58] = settr(509,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(509,0,57,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][57]	= settr(508,2,99,646,0,"assert((((Untitled_MSC_0_sig1_param0.status2==0)||(Untitled_MSC_0_sig1_param0.status2==1))||(Untitled_MSC_0_sig1_param0.status2==2)))", 1, 2500, 0);
	T = trans[ 2][99] = settr(550,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(550,0,61,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][61]	= settr(512,2,96,647,647,"transition_id = 0", 1, 2500, 0); /* m: 62 -> 0,96 */
	reached2[62] = 1;
	trans[2][62]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[2][97]	= settr(548,2,96,1,0,".(goto)", 1, 2500, 0);
	T = trans[2][96] = settr(547,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(547,2,93,0,0,"DO", 1, 2500, 0);
	T = trans[2][93] = settr(544,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(544,2,63,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(544,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(544,2,69,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(544,2,81,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(544,2,85,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(544,2,89,0,0,"IF", 1, 2500, 0);
	trans[2][63]	= settr(514,2,105,648,648,"((transition_id==-(1)))", 1, 2500, 0);
	trans[2][64]	= settr(515,2,105,1,0,"goto :b10", 1, 2500, 0);
	trans[2][94]	= settr(545,2,95,1,0,".(goto)", 1, 2500, 0);
	trans[2][65]	= settr(516,2,95,649,649,"((transition_id==0))", 1, 2500, 0); /* m: 66 -> 95,0 */
	reached2[66] = 1;
	trans[2][66]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][67]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][68]	= settr(519,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][69]	= settr(520,2,76,650,650,"((transition_id==1))", 1, 2500, 0);
	T = trans[2][76] = settr(527,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(527,2,70,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(527,2,74,0,0,"IF", 1, 2500, 0);
	trans[2][70]	= settr(521,2,95,651,651,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 71 -> 95,0 */
	reached2[71] = 1;
	trans[2][71]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][72]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[2][73]	= settr(524,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][77]	= settr(528,2,78,1,0,".(goto)", 1, 2500, 0); /* m: 78 -> 0,95 */
	reached2[78] = 1;
	trans[2][74]	= settr(525,2,75,2,0,"else", 1, 2500, 0);
	trans[2][75]	= settr(526,2,95,652,652,"(1)", 1, 2500, 0); /* m: 78 -> 95,0 */
	reached2[78] = 1;
	trans[2][78]	= settr(529,2,95,653,653,"transition_id = -(1)", 1, 2500, 0); /* m: 79 -> 0,95 */
	reached2[79] = 1;
	trans[2][79]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][80]	= settr(531,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][81]	= settr(532,2,95,654,654,"((transition_id==2))", 1, 2500, 0); /* m: 82 -> 95,0 */
	reached2[82] = 1;
	trans[2][82]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][83]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][84]	= settr(535,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][85]	= settr(536,2,95,655,655,"((transition_id==3))", 1, 2500, 0); /* m: 86 -> 95,0 */
	reached2[86] = 1;
	trans[2][86]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][87]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[2][88]	= settr(539,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][89]	= settr(540,2,95,656,656,"((transition_id==4))", 1, 2500, 0); /* m: 90 -> 95,0 */
	reached2[90] = 1;
	trans[2][90]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][91]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[2][92]	= settr(543,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][95]	= settr(546,2,96,1,0,"(1)", 1, 2500, 0);
	trans[2][98]	= settr(549,2,105,1,0,"break", 1, 2500, 0);
	trans[2][100]	= settr(551,2,101,2,0,"else", 1, 2500, 0);
	trans[2][101]	= settr(552,2,105,1,0,"(1)", 1, 2500, 0);
	trans[2][105]	= settr(556,2,109,657,657,"Untitled_msc_lock!1", 1, 2500, 0);
	trans[2][106]	= settr(557,2,109,1,0,"goto Egse_systemtm_loop", 1, 2500, 0);
	trans[2][110]	= settr(561,0,112,1,0,".(goto)", 1, 2500, 0);
	trans[2][107]	= settr(558,2,108,658,0,"(empty(Egse_systemTm_channel))", 1, 2500, 0);
	trans[2][108]	= settr(559,0,112,659,659,"(1)", 1, 2500, 0); /* m: 110 -> 112,0 */
	reached2[110] = 1;
	trans[2][114]	= settr(565,0,115,1,0,"break", 0, 2, 0);
	trans[2][115]	= settr(566,0,0,660,660,"-end-", 0, 3500, 0);

	/* proctype 1: Actuator2_actuatorTc */

	trans[1] = (Trans **) emalloc(227*sizeof(Trans *));

	trans[1][1]	= settr(226,0,223,661,0,"(inited)", 1, 2, 0);
	trans[1][224]	= settr(449,0,223,1,0,".(goto)", 0, 2, 0);
	T = trans[1][223] = settr(448,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(448,0,222,0,0,"DO", 0, 2, 0);
	T = trans[ 1][222] = settr(447,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(447,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[1][2]	= settr(227,2,3,662,0,"(nempty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][3]	= settr(228,2,219,663,663,"Actuator2_lock?_", 1, 2500, 0);
	T = trans[1][219] = settr(444,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(444,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(444,2,217,0,0,"IF", 1, 2500, 0);
	trans[1][4]	= settr(229,2,5,664,0,"(nempty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][5]	= settr(230,2,215,665,665,"Actuator2_actuatorTc_channel?Actuator2_actuatortc_signal_parameter", 1, 2500, 0); /* m: 6 -> 215,0 */
	reached1[6] = 1;
	trans[1][6]	= settr(0,0,0,0,0,"Actuator2_actuatortc_channel_used = 1",0,0,0);
	T = trans[ 1][215] = settr(440,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(440,0,213,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[1][213] = settr(438,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,75,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,143,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(438,2,211,0,0,"IF", 1, 2500, 0);
	trans[1][7]	= settr(232,2,11,666,0,"((global_state.actuator2.state==0))", 1, 2500, 0);
	T = trans[ 1][11] = settr(236,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(236,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][8]	= settr(233,2,10,667,667,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][10] = settr(235,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(235,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][9]	= settr(234,2,74,668,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][74] = settr(299,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(299,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][12]	= settr(237,2,71,669,669,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,71 */
	reached1[13] = 1;
	trans[1][13]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[1][72]	= settr(297,2,71,1,0,".(goto)", 1, 2500, 0);
	T = trans[1][71] = settr(296,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(296,2,68,0,0,"DO", 1, 2500, 0);
	T = trans[1][68] = settr(293,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(293,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(293,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(293,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(293,2,40,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(293,2,54,0,0,"IF", 1, 2500, 0);
	trans[1][14]	= settr(239,2,219,670,670,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][15]	= settr(240,2,219,1,0,"goto :b5", 1, 2500, 0);
	trans[1][69]	= settr(294,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][16]	= settr(241,2,70,671,671,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 70,0 */
	reached1[17] = 1;
	trans[1][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][18]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][19]	= settr(244,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][20]	= settr(245,2,22,672,672,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][22] = settr(247,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(247,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][21]	= settr(246,2,38,1,0,"(1)", 1, 2500, 0);
	T = trans[1][38] = settr(263,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(263,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(263,2,29,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(263,2,35,0,0,"IF", 1, 2500, 0);
	trans[1][23]	= settr(248,2,25,673,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][25] = settr(250,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(250,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][24]	= settr(249,2,70,674,674,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 26 -> 70,0 */
	reached1[26] = 1;
	trans[1][26]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][27]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][28]	= settr(253,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][39]	= settr(264,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][29]	= settr(254,2,31,675,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][31] = settr(256,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(256,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][30]	= settr(255,2,70,676,676,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 32 -> 70,0 */
	reached1[32] = 1;
	trans[1][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][33]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][34]	= settr(259,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][35]	= settr(260,2,36,2,0,"else", 1, 2500, 0);
	trans[1][36]	= settr(261,2,70,677,677,"transition_id = -(1)", 1, 2500, 0);
	trans[1][37]	= settr(262,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][40]	= settr(265,2,42,678,678,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][42] = settr(267,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(267,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][41]	= settr(266,2,52,1,0,"(1)", 1, 2500, 0);
	T = trans[1][52] = settr(277,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(277,2,43,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(277,2,49,0,0,"IF", 1, 2500, 0);
	trans[1][43]	= settr(268,2,45,679,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][45] = settr(270,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(270,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][44]	= settr(269,2,70,680,680,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 46 -> 70,0 */
	reached1[46] = 1;
	trans[1][46]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][47]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][48]	= settr(273,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][53]	= settr(278,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][49]	= settr(274,2,50,2,0,"else", 1, 2500, 0);
	trans[1][50]	= settr(275,2,70,681,681,"transition_id = -(1)", 1, 2500, 0);
	trans[1][51]	= settr(276,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][54]	= settr(279,2,56,682,682,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][56] = settr(281,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(281,0,55,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][55]	= settr(280,2,66,1,0,"(1)", 1, 2500, 0);
	T = trans[1][66] = settr(291,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(291,2,57,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(291,2,63,0,0,"IF", 1, 2500, 0);
	trans[1][57]	= settr(282,2,59,683,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][59] = settr(284,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(284,0,58,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][58]	= settr(283,2,70,684,684,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 60 -> 70,0 */
	reached1[60] = 1;
	trans[1][60]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][61]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][62]	= settr(287,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][67]	= settr(292,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][63]	= settr(288,2,64,2,0,"else", 1, 2500, 0);
	trans[1][64]	= settr(289,2,70,685,685,"transition_id = -(1)", 1, 2500, 0);
	trans[1][65]	= settr(290,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][70]	= settr(295,2,71,1,0,"(1)", 1, 2500, 0);
	trans[1][73]	= settr(298,2,219,1,0,"break", 1, 2500, 0);
	trans[1][214]	= settr(439,2,219,1,0,".(goto)", 1, 2500, 0);
	trans[1][75]	= settr(300,2,79,686,0,"((global_state.actuator2.state==1))", 1, 2500, 0);
	T = trans[ 1][79] = settr(304,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(304,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][76]	= settr(301,2,78,687,687,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][78] = settr(303,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(303,0,77,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][77]	= settr(302,2,142,688,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][142] = settr(367,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(367,0,80,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][80]	= settr(305,2,139,689,689,"transition_id = 0", 1, 2500, 0); /* m: 81 -> 0,139 */
	reached1[81] = 1;
	trans[1][81]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[1][140]	= settr(365,2,139,1,0,".(goto)", 1, 2500, 0);
	T = trans[1][139] = settr(364,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(364,2,136,0,0,"DO", 1, 2500, 0);
	T = trans[1][136] = settr(361,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(361,2,82,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(361,2,84,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(361,2,88,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(361,2,108,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(361,2,122,0,0,"IF", 1, 2500, 0);
	trans[1][82]	= settr(307,2,219,690,690,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][83]	= settr(308,2,219,1,0,"goto :b6", 1, 2500, 0);
	trans[1][137]	= settr(362,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][84]	= settr(309,2,138,691,691,"((transition_id==0))", 1, 2500, 0); /* m: 85 -> 138,0 */
	reached1[85] = 1;
	trans[1][85]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][86]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][87]	= settr(312,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][88]	= settr(313,2,90,692,692,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][90] = settr(315,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(315,0,89,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][89]	= settr(314,2,106,1,0,"(1)", 1, 2500, 0);
	T = trans[1][106] = settr(331,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(331,2,91,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(331,2,97,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(331,2,103,0,0,"IF", 1, 2500, 0);
	trans[1][91]	= settr(316,2,93,693,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][93] = settr(318,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(318,0,92,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][92]	= settr(317,2,138,694,694,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 94 -> 138,0 */
	reached1[94] = 1;
	trans[1][94]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][95]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][96]	= settr(321,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][107]	= settr(332,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][97]	= settr(322,2,99,695,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][99] = settr(324,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(324,0,98,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][98]	= settr(323,2,138,696,696,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 100 -> 138,0 */
	reached1[100] = 1;
	trans[1][100]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][101]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][102]	= settr(327,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][103]	= settr(328,2,104,2,0,"else", 1, 2500, 0);
	trans[1][104]	= settr(329,2,138,697,697,"transition_id = -(1)", 1, 2500, 0);
	trans[1][105]	= settr(330,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][108]	= settr(333,2,110,698,698,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][110] = settr(335,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(335,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][109]	= settr(334,2,120,1,0,"(1)", 1, 2500, 0);
	T = trans[1][120] = settr(345,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(345,2,111,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(345,2,117,0,0,"IF", 1, 2500, 0);
	trans[1][111]	= settr(336,2,113,699,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][113] = settr(338,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(338,0,112,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][112]	= settr(337,2,138,700,700,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 114 -> 138,0 */
	reached1[114] = 1;
	trans[1][114]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][115]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][116]	= settr(341,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][121]	= settr(346,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][117]	= settr(342,2,118,2,0,"else", 1, 2500, 0);
	trans[1][118]	= settr(343,2,138,701,701,"transition_id = -(1)", 1, 2500, 0);
	trans[1][119]	= settr(344,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][122]	= settr(347,2,124,702,702,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][124] = settr(349,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(349,0,123,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][123]	= settr(348,2,134,1,0,"(1)", 1, 2500, 0);
	T = trans[1][134] = settr(359,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(359,2,125,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(359,2,131,0,0,"IF", 1, 2500, 0);
	trans[1][125]	= settr(350,2,127,703,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][127] = settr(352,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(352,0,126,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][126]	= settr(351,2,138,704,704,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 128 -> 138,0 */
	reached1[128] = 1;
	trans[1][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][129]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][130]	= settr(355,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][135]	= settr(360,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][131]	= settr(356,2,132,2,0,"else", 1, 2500, 0);
	trans[1][132]	= settr(357,2,138,705,705,"transition_id = -(1)", 1, 2500, 0);
	trans[1][133]	= settr(358,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][138]	= settr(363,2,139,1,0,"(1)", 1, 2500, 0);
	trans[1][141]	= settr(366,2,219,1,0,"break", 1, 2500, 0);
	trans[1][143]	= settr(368,2,147,706,0,"((global_state.actuator2.state==2))", 1, 2500, 0);
	T = trans[ 1][147] = settr(372,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(372,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][144]	= settr(369,2,146,707,707,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][146] = settr(371,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(371,0,145,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][145]	= settr(370,2,210,708,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][210] = settr(435,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(435,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][148]	= settr(373,2,207,709,709,"transition_id = 0", 1, 2500, 0); /* m: 149 -> 0,207 */
	reached1[149] = 1;
	trans[1][149]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[1][208]	= settr(433,2,207,1,0,".(goto)", 1, 2500, 0);
	T = trans[1][207] = settr(432,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(432,2,204,0,0,"DO", 1, 2500, 0);
	T = trans[1][204] = settr(429,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(429,2,150,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(429,2,152,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(429,2,156,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(429,2,176,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(429,2,190,0,0,"IF", 1, 2500, 0);
	trans[1][150]	= settr(375,2,219,710,710,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][151]	= settr(376,2,219,1,0,"goto :b7", 1, 2500, 0);
	trans[1][205]	= settr(430,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][152]	= settr(377,2,206,711,711,"((transition_id==0))", 1, 2500, 0); /* m: 153 -> 206,0 */
	reached1[153] = 1;
	trans[1][153]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][154]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][155]	= settr(380,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][156]	= settr(381,2,158,712,712,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][158] = settr(383,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(383,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][157]	= settr(382,2,174,1,0,"(1)", 1, 2500, 0);
	T = trans[1][174] = settr(399,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(399,2,159,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(399,2,165,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(399,2,171,0,0,"IF", 1, 2500, 0);
	trans[1][159]	= settr(384,2,161,713,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][161] = settr(386,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(386,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][160]	= settr(385,2,206,714,714,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 162 -> 206,0 */
	reached1[162] = 1;
	trans[1][162]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][163]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][164]	= settr(389,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][175]	= settr(400,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][165]	= settr(390,2,167,715,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][167] = settr(392,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(392,0,166,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][166]	= settr(391,2,206,716,716,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 168 -> 206,0 */
	reached1[168] = 1;
	trans[1][168]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][169]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][170]	= settr(395,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][171]	= settr(396,2,172,2,0,"else", 1, 2500, 0);
	trans[1][172]	= settr(397,2,206,717,717,"transition_id = -(1)", 1, 2500, 0);
	trans[1][173]	= settr(398,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][176]	= settr(401,2,178,718,718,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][178] = settr(403,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(403,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][177]	= settr(402,2,188,1,0,"(1)", 1, 2500, 0);
	T = trans[1][188] = settr(413,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(413,2,179,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(413,2,185,0,0,"IF", 1, 2500, 0);
	trans[1][179]	= settr(404,2,181,719,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][181] = settr(406,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(406,0,180,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][180]	= settr(405,2,206,720,720,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 182 -> 206,0 */
	reached1[182] = 1;
	trans[1][182]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][183]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][184]	= settr(409,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][189]	= settr(414,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][185]	= settr(410,2,186,2,0,"else", 1, 2500, 0);
	trans[1][186]	= settr(411,2,206,721,721,"transition_id = -(1)", 1, 2500, 0);
	trans[1][187]	= settr(412,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][190]	= settr(415,2,192,722,722,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][192] = settr(417,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(417,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][191]	= settr(416,2,202,1,0,"(1)", 1, 2500, 0);
	T = trans[1][202] = settr(427,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(427,2,193,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(427,2,199,0,0,"IF", 1, 2500, 0);
	trans[1][193]	= settr(418,2,195,723,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][195] = settr(420,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(420,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][194]	= settr(419,2,206,724,724,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 196 -> 206,0 */
	reached1[196] = 1;
	trans[1][196]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][197]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][198]	= settr(423,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][203]	= settr(428,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][199]	= settr(424,2,200,2,0,"else", 1, 2500, 0);
	trans[1][200]	= settr(425,2,206,725,725,"transition_id = -(1)", 1, 2500, 0);
	trans[1][201]	= settr(426,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][206]	= settr(431,2,207,1,0,"(1)", 1, 2500, 0);
	trans[1][209]	= settr(434,2,219,1,0,"break", 1, 2500, 0);
	trans[1][211]	= settr(436,2,212,2,0,"else", 1, 2500, 0);
	trans[1][212]	= settr(437,2,219,1,0,"(1)", 1, 2500, 0);
	trans[1][216]	= settr(441,2,219,1,0,"goto Actuator2_actuatortc_loop", 1, 2500, 0);
	trans[1][220]	= settr(445,2,221,1,0,".(goto)", 1, 2500, 0);
	trans[1][217]	= settr(442,2,218,726,0,"(empty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][218]	= settr(443,2,221,727,727,"(1)", 1, 2500, 0); /* m: 220 -> 221,0 */
	reached1[220] = 1;
	trans[1][221]	= settr(446,0,223,728,728,"Actuator2_lock!1", 1, 2500, 0);
	trans[1][225]	= settr(450,0,226,1,0,"break", 0, 2, 0);
	trans[1][226]	= settr(451,0,0,729,729,"-end-", 0, 3500, 0);

	/* proctype 0: Actuator1_actuatorTc */

	trans[0] = (Trans **) emalloc(227*sizeof(Trans *));

	trans[0][1]	= settr(0,0,223,730,0,"(inited)", 1, 2, 0);
	trans[0][224]	= settr(223,0,223,1,0,".(goto)", 0, 2, 0);
	T = trans[0][223] = settr(222,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(222,0,222,0,0,"DO", 0, 2, 0);
	T = trans[ 0][222] = settr(221,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(221,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[0][2]	= settr(1,2,3,731,0,"(nempty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][3]	= settr(2,2,219,732,732,"Actuator1_lock?_", 1, 2500, 0);
	T = trans[0][219] = settr(218,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(218,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(218,2,217,0,0,"IF", 1, 2500, 0);
	trans[0][4]	= settr(3,2,5,733,0,"(nempty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][5]	= settr(4,2,215,734,734,"Actuator1_actuatorTc_channel?Actuator1_actuatortc_signal_parameter", 1, 2500, 0); /* m: 6 -> 215,0 */
	reached0[6] = 1;
	trans[0][6]	= settr(0,0,0,0,0,"Actuator1_actuatortc_channel_used = 1",0,0,0);
	T = trans[ 0][215] = settr(214,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(214,0,213,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[0][213] = settr(212,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,75,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,143,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(212,2,211,0,0,"IF", 1, 2500, 0);
	trans[0][7]	= settr(6,2,11,735,0,"((global_state.actuator1.state==0))", 1, 2500, 0);
	T = trans[ 0][11] = settr(10,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(10,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][8]	= settr(7,2,10,736,736,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][10] = settr(9,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(9,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][9]	= settr(8,2,74,737,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][74] = settr(73,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(73,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][12]	= settr(11,2,71,738,738,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,71 */
	reached0[13] = 1;
	trans[0][13]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[0][72]	= settr(71,2,71,1,0,".(goto)", 1, 2500, 0);
	T = trans[0][71] = settr(70,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(70,2,68,0,0,"DO", 1, 2500, 0);
	T = trans[0][68] = settr(67,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(67,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(67,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(67,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(67,2,40,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(67,2,54,0,0,"IF", 1, 2500, 0);
	trans[0][14]	= settr(13,2,219,739,739,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][15]	= settr(14,2,219,1,0,"goto :b1", 1, 2500, 0);
	trans[0][69]	= settr(68,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][16]	= settr(15,2,70,740,740,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 70,0 */
	reached0[17] = 1;
	trans[0][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][18]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][19]	= settr(18,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][20]	= settr(19,2,22,741,741,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][22] = settr(21,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(21,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][21]	= settr(20,2,38,1,0,"(1)", 1, 2500, 0);
	T = trans[0][38] = settr(37,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(37,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(37,2,29,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(37,2,35,0,0,"IF", 1, 2500, 0);
	trans[0][23]	= settr(22,2,25,742,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][25] = settr(24,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(24,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][24]	= settr(23,2,70,743,743,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 26 -> 70,0 */
	reached0[26] = 1;
	trans[0][26]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][27]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][28]	= settr(27,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][39]	= settr(38,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][29]	= settr(28,2,31,744,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][31] = settr(30,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(30,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][30]	= settr(29,2,70,745,745,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 32 -> 70,0 */
	reached0[32] = 1;
	trans[0][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][33]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][34]	= settr(33,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][35]	= settr(34,2,36,2,0,"else", 1, 2500, 0);
	trans[0][36]	= settr(35,2,70,746,746,"transition_id = -(1)", 1, 2500, 0);
	trans[0][37]	= settr(36,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][40]	= settr(39,2,42,747,747,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][42] = settr(41,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(41,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][41]	= settr(40,2,52,1,0,"(1)", 1, 2500, 0);
	T = trans[0][52] = settr(51,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(51,2,43,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(51,2,49,0,0,"IF", 1, 2500, 0);
	trans[0][43]	= settr(42,2,45,748,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][45] = settr(44,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(44,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][44]	= settr(43,2,70,749,749,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 46 -> 70,0 */
	reached0[46] = 1;
	trans[0][46]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][47]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][48]	= settr(47,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][53]	= settr(52,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][49]	= settr(48,2,50,2,0,"else", 1, 2500, 0);
	trans[0][50]	= settr(49,2,70,750,750,"transition_id = -(1)", 1, 2500, 0);
	trans[0][51]	= settr(50,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][54]	= settr(53,2,56,751,751,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][56] = settr(55,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(55,0,55,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][55]	= settr(54,2,66,1,0,"(1)", 1, 2500, 0);
	T = trans[0][66] = settr(65,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(65,2,57,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(65,2,63,0,0,"IF", 1, 2500, 0);
	trans[0][57]	= settr(56,2,59,752,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][59] = settr(58,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(58,0,58,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][58]	= settr(57,2,70,753,753,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 60 -> 70,0 */
	reached0[60] = 1;
	trans[0][60]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][61]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][62]	= settr(61,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][67]	= settr(66,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][63]	= settr(62,2,64,2,0,"else", 1, 2500, 0);
	trans[0][64]	= settr(63,2,70,754,754,"transition_id = -(1)", 1, 2500, 0);
	trans[0][65]	= settr(64,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][70]	= settr(69,2,71,1,0,"(1)", 1, 2500, 0);
	trans[0][73]	= settr(72,2,219,1,0,"break", 1, 2500, 0);
	trans[0][214]	= settr(213,2,219,1,0,".(goto)", 1, 2500, 0);
	trans[0][75]	= settr(74,2,79,755,0,"((global_state.actuator1.state==1))", 1, 2500, 0);
	T = trans[ 0][79] = settr(78,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(78,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][76]	= settr(75,2,78,756,756,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][78] = settr(77,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(77,0,77,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][77]	= settr(76,2,142,757,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][142] = settr(141,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(141,0,80,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][80]	= settr(79,2,139,758,758,"transition_id = 0", 1, 2500, 0); /* m: 81 -> 0,139 */
	reached0[81] = 1;
	trans[0][81]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[0][140]	= settr(139,2,139,1,0,".(goto)", 1, 2500, 0);
	T = trans[0][139] = settr(138,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(138,2,136,0,0,"DO", 1, 2500, 0);
	T = trans[0][136] = settr(135,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(135,2,82,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(135,2,84,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(135,2,88,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(135,2,108,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(135,2,122,0,0,"IF", 1, 2500, 0);
	trans[0][82]	= settr(81,2,219,759,759,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][83]	= settr(82,2,219,1,0,"goto :b2", 1, 2500, 0);
	trans[0][137]	= settr(136,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][84]	= settr(83,2,138,760,760,"((transition_id==0))", 1, 2500, 0); /* m: 85 -> 138,0 */
	reached0[85] = 1;
	trans[0][85]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][86]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][87]	= settr(86,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][88]	= settr(87,2,90,761,761,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][90] = settr(89,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(89,0,89,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][89]	= settr(88,2,106,1,0,"(1)", 1, 2500, 0);
	T = trans[0][106] = settr(105,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(105,2,91,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(105,2,97,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(105,2,103,0,0,"IF", 1, 2500, 0);
	trans[0][91]	= settr(90,2,93,762,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][93] = settr(92,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(92,0,92,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][92]	= settr(91,2,138,763,763,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 94 -> 138,0 */
	reached0[94] = 1;
	trans[0][94]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][95]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][96]	= settr(95,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][107]	= settr(106,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][97]	= settr(96,2,99,764,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][99] = settr(98,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(98,0,98,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][98]	= settr(97,2,138,765,765,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 100 -> 138,0 */
	reached0[100] = 1;
	trans[0][100]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][101]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][102]	= settr(101,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][103]	= settr(102,2,104,2,0,"else", 1, 2500, 0);
	trans[0][104]	= settr(103,2,138,766,766,"transition_id = -(1)", 1, 2500, 0);
	trans[0][105]	= settr(104,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][108]	= settr(107,2,110,767,767,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][110] = settr(109,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(109,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][109]	= settr(108,2,120,1,0,"(1)", 1, 2500, 0);
	T = trans[0][120] = settr(119,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(119,2,111,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(119,2,117,0,0,"IF", 1, 2500, 0);
	trans[0][111]	= settr(110,2,113,768,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][113] = settr(112,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(112,0,112,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][112]	= settr(111,2,138,769,769,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 114 -> 138,0 */
	reached0[114] = 1;
	trans[0][114]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][115]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][116]	= settr(115,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][121]	= settr(120,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][117]	= settr(116,2,118,2,0,"else", 1, 2500, 0);
	trans[0][118]	= settr(117,2,138,770,770,"transition_id = -(1)", 1, 2500, 0);
	trans[0][119]	= settr(118,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][122]	= settr(121,2,124,771,771,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][124] = settr(123,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(123,0,123,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][123]	= settr(122,2,134,1,0,"(1)", 1, 2500, 0);
	T = trans[0][134] = settr(133,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(133,2,125,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(133,2,131,0,0,"IF", 1, 2500, 0);
	trans[0][125]	= settr(124,2,127,772,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][127] = settr(126,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(126,0,126,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][126]	= settr(125,2,138,773,773,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 128 -> 138,0 */
	reached0[128] = 1;
	trans[0][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][129]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][130]	= settr(129,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][135]	= settr(134,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][131]	= settr(130,2,132,2,0,"else", 1, 2500, 0);
	trans[0][132]	= settr(131,2,138,774,774,"transition_id = -(1)", 1, 2500, 0);
	trans[0][133]	= settr(132,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][138]	= settr(137,2,139,1,0,"(1)", 1, 2500, 0);
	trans[0][141]	= settr(140,2,219,1,0,"break", 1, 2500, 0);
	trans[0][143]	= settr(142,2,147,775,0,"((global_state.actuator1.state==2))", 1, 2500, 0);
	T = trans[ 0][147] = settr(146,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(146,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][144]	= settr(143,2,146,776,776,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][146] = settr(145,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(145,0,145,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][145]	= settr(144,2,210,777,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][210] = settr(209,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(209,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][148]	= settr(147,2,207,778,778,"transition_id = 0", 1, 2500, 0); /* m: 149 -> 0,207 */
	reached0[149] = 1;
	trans[0][149]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[0][208]	= settr(207,2,207,1,0,".(goto)", 1, 2500, 0);
	T = trans[0][207] = settr(206,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(206,2,204,0,0,"DO", 1, 2500, 0);
	T = trans[0][204] = settr(203,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(203,2,150,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(203,2,152,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(203,2,156,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(203,2,176,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(203,2,190,0,0,"IF", 1, 2500, 0);
	trans[0][150]	= settr(149,2,219,779,779,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][151]	= settr(150,2,219,1,0,"goto :b3", 1, 2500, 0);
	trans[0][205]	= settr(204,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][152]	= settr(151,2,206,780,780,"((transition_id==0))", 1, 2500, 0); /* m: 153 -> 206,0 */
	reached0[153] = 1;
	trans[0][153]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][154]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][155]	= settr(154,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][156]	= settr(155,2,158,781,781,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][158] = settr(157,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(157,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][157]	= settr(156,2,174,1,0,"(1)", 1, 2500, 0);
	T = trans[0][174] = settr(173,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(173,2,159,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(173,2,165,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(173,2,171,0,0,"IF", 1, 2500, 0);
	trans[0][159]	= settr(158,2,161,782,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][161] = settr(160,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(160,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][160]	= settr(159,2,206,783,783,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 162 -> 206,0 */
	reached0[162] = 1;
	trans[0][162]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][163]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][164]	= settr(163,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][175]	= settr(174,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][165]	= settr(164,2,167,784,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][167] = settr(166,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(166,0,166,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][166]	= settr(165,2,206,785,785,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 168 -> 206,0 */
	reached0[168] = 1;
	trans[0][168]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][169]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][170]	= settr(169,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][171]	= settr(170,2,172,2,0,"else", 1, 2500, 0);
	trans[0][172]	= settr(171,2,206,786,786,"transition_id = -(1)", 1, 2500, 0);
	trans[0][173]	= settr(172,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][176]	= settr(175,2,178,787,787,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][178] = settr(177,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(177,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][177]	= settr(176,2,188,1,0,"(1)", 1, 2500, 0);
	T = trans[0][188] = settr(187,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(187,2,179,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(187,2,185,0,0,"IF", 1, 2500, 0);
	trans[0][179]	= settr(178,2,181,788,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][181] = settr(180,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(180,0,180,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][180]	= settr(179,2,206,789,789,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 182 -> 206,0 */
	reached0[182] = 1;
	trans[0][182]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][183]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][184]	= settr(183,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][189]	= settr(188,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][185]	= settr(184,2,186,2,0,"else", 1, 2500, 0);
	trans[0][186]	= settr(185,2,206,790,790,"transition_id = -(1)", 1, 2500, 0);
	trans[0][187]	= settr(186,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][190]	= settr(189,2,192,791,791,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][192] = settr(191,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(191,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][191]	= settr(190,2,202,1,0,"(1)", 1, 2500, 0);
	T = trans[0][202] = settr(201,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(201,2,193,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(201,2,199,0,0,"IF", 1, 2500, 0);
	trans[0][193]	= settr(192,2,195,792,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][195] = settr(194,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(194,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][194]	= settr(193,2,206,793,793,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 196 -> 206,0 */
	reached0[196] = 1;
	trans[0][196]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][197]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][198]	= settr(197,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][203]	= settr(202,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][199]	= settr(198,2,200,2,0,"else", 1, 2500, 0);
	trans[0][200]	= settr(199,2,206,794,794,"transition_id = -(1)", 1, 2500, 0);
	trans[0][201]	= settr(200,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][206]	= settr(205,2,207,1,0,"(1)", 1, 2500, 0);
	trans[0][209]	= settr(208,2,219,1,0,"break", 1, 2500, 0);
	trans[0][211]	= settr(210,2,212,2,0,"else", 1, 2500, 0);
	trans[0][212]	= settr(211,2,219,1,0,"(1)", 1, 2500, 0);
	trans[0][216]	= settr(215,2,219,1,0,"goto Actuator1_actuatortc_loop", 1, 2500, 0);
	trans[0][220]	= settr(219,2,221,1,0,".(goto)", 1, 2500, 0);
	trans[0][217]	= settr(216,2,218,795,0,"(empty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][218]	= settr(217,2,221,796,796,"(1)", 1, 2500, 0); /* m: 220 -> 221,0 */
	reached0[220] = 1;
	trans[0][221]	= settr(220,0,223,797,797,"Actuator1_lock!1", 1, 2500, 0);
	trans[0][225]	= settr(224,0,226,1,0,"break", 0, 2, 0);
	trans[0][226]	= settr(225,0,0,798,798,"-end-", 0, 3500, 0);
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
