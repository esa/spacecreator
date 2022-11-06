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

	trans[10][6]	= settr(2342,0,5,1,0,".(goto)", 0, 2, 0);
	T = trans[10][5] = settr(2341,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2341,0,1,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(2341,0,3,0,0,"DO", 0, 2, 0);
	trans[10][1]	= settr(2337,0,16,3,0,"(inited)", 1, 2, 0);
	trans[10][2]	= settr(2338,0,16,1,0,"goto :b41", 0, 2, 0);
	trans[10][3]	= settr(2339,0,5,2,0,"else", 0, 2, 0);
	trans[10][4]	= settr(2340,0,5,1,0,"goto start", 0, 2, 0);
	trans[10][7]	= settr(2343,0,16,1,0,"break", 0, 2, 0);
	trans[10][17]	= settr(2353,0,16,1,0,".(goto)", 0, 2, 0);
	T = trans[10][16] = settr(2352,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2352,0,11,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2352,0,12,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(2352,0,14,0,0,"DO", 0, 2, 0);
	T = trans[ 10][11] = settr(2347,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(2347,2,8,0,0,"ATOMIC", 1, 2, 0);
	trans[10][8]	= settr(2344,0,16,4,4,"((global_state.untitled_msc.state==3))", 1, 2, 0); /* m: 9 -> 16,0 */
	reached10[9] = 1;
	trans[10][9]	= settr(0,0,0,0,0,"printf('Observer entered errorstate: untitled_msc\\n')",0,0,0);
	trans[10][10]	= settr(0,0,0,0,0,"assert(!((global_state.untitled_msc.state==3)))",0,0,0);
	trans[10][12]	= settr(2348,0,25,5,0,"((global_state.untitled_msc.state==1))", 1, 2, 0);
	trans[10][13]	= settr(2349,0,25,1,0,"goto state_0", 0, 2, 0);
	trans[10][14]	= settr(2350,0,16,2,0,"else", 0, 2, 0);
	trans[10][15]	= settr(2351,0,16,1,0,"goto accept_all", 0, 2, 0);
	trans[10][18]	= settr(2354,0,25,1,0,"break", 0, 2, 0);
	trans[10][26]	= settr(2362,0,25,1,0,".(goto)", 0, 2, 0);
	T = trans[10][25] = settr(2361,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(2361,0,22,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(2361,0,23,0,0,"DO", 0, 2, 0);
	T = trans[ 10][22] = settr(2358,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(2358,2,19,0,0,"ATOMIC", 1, 2, 0);
	trans[10][19]	= settr(2355,0,25,6,6,"((global_state.untitled_msc.state==3))", 1, 2, 0); /* m: 20 -> 25,0 */
	reached10[20] = 1;
	trans[10][20]	= settr(0,0,0,0,0,"printf('Observer entered errorstate: untitled_msc\\n')",0,0,0);
	trans[10][21]	= settr(0,0,0,0,0,"assert(!((global_state.untitled_msc.state==3)))",0,0,0);
	trans[10][23]	= settr(2359,0,25,2,0,"else", 0, 2, 0);
	trans[10][24]	= settr(2360,0,25,1,0,"goto state_0", 0, 2, 0);
	trans[10][27]	= settr(2363,0,28,1,0,"break", 0, 2, 0);
	trans[10][28]	= settr(2364,0,0,7,7,"-end-", 0, 3500, 0);

	/* proctype 9: :init: */

	trans[9] = (Trans **) emalloc(342*sizeof(Trans *));

	T = trans[ 9][340] = settr(2335,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(2335,2,5,0,0,"ATOMIC", 1, 2500, 0);
	T = trans[ 9][5] = settr(2000,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2000,0,4,0,0,"sub-sequence", 1, 2500, 0);
/*->*/	trans[9][4]	= settr(1999,34,73,8,8,"D_STEP20551", 1, 2, 0);
	T = trans[ 9][73] = settr(2068,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2068,0,9,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][9] = settr(2004,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2004,0,6,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][6]	= settr(2001,2,8,9,9,"global_state.actuator1.sender = 6", 1, 2500, 0);
	T = trans[ 9][8] = settr(2003,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2003,0,7,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][7]	= settr(2002,2,72,10,0,"assert((((((((global_state.actuator1.sender==0)||(global_state.actuator1.sender==1))||(global_state.actuator1.sender==2))||(global_state.actuator1.sender==3))||(global_state.actuator1.sender==4))||(global_state.actuator1.sender==5))||(global_state.actuator1.sender==6)))", 1, 2500, 0);
	T = trans[ 9][72] = settr(2067,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2067,0,10,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][10]	= settr(2005,2,69,11,11,"transition_id = 0", 1, 2500, 0); /* m: 11 -> 0,69 */
	reached9[11] = 1;
	trans[9][11]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][70]	= settr(2065,2,69,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][69] = settr(2064,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2064,2,66,0,0,"DO", 1, 2500, 0);
	T = trans[9][66] = settr(2061,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2061,2,12,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2061,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2061,2,18,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2061,2,38,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2061,2,52,0,0,"IF", 1, 2500, 0);
	trans[9][12]	= settr(2007,2,74,12,12,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][13]	= settr(2008,2,74,1,0,"goto :b37", 1, 2500, 0);
	trans[9][67]	= settr(2062,2,68,1,0,".(goto)", 1, 2500, 0);
	trans[9][14]	= settr(2009,2,68,13,13,"((transition_id==0))", 1, 2500, 0); /* m: 15 -> 68,0 */
	reached9[15] = 1;
	trans[9][15]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][16]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][17]	= settr(2012,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][18]	= settr(2013,2,20,14,14,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][20] = settr(2015,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2015,0,19,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][19]	= settr(2014,2,36,1,0,"(1)", 1, 2500, 0);
	T = trans[9][36] = settr(2031,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,21,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,27,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2031,2,33,0,0,"IF", 1, 2500, 0);
	trans[9][21]	= settr(2016,2,23,15,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 9][23] = settr(2018,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2018,0,22,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][22]	= settr(2017,2,68,16,16,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 24 -> 68,0 */
	reached9[24] = 1;
	trans[9][24]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][25]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][26]	= settr(2021,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][37]	= settr(2032,2,68,1,0,".(goto)", 1, 2500, 0);
	trans[9][27]	= settr(2022,2,29,17,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 9][29] = settr(2024,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2024,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][28]	= settr(2023,2,68,18,18,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 30 -> 68,0 */
	reached9[30] = 1;
	trans[9][30]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][31]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[9][32]	= settr(2027,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][33]	= settr(2028,2,34,2,0,"else", 1, 2500, 0);
	trans[9][34]	= settr(2029,2,68,19,19,"transition_id = -(1)", 1, 2500, 0);
	trans[9][35]	= settr(2030,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][38]	= settr(2033,2,40,20,20,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][40] = settr(2035,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2035,0,39,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][39]	= settr(2034,2,50,1,0,"(1)", 1, 2500, 0);
	T = trans[9][50] = settr(2045,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2045,2,41,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2045,2,47,0,0,"IF", 1, 2500, 0);
	trans[9][41]	= settr(2036,2,43,21,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 9][43] = settr(2038,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2038,0,42,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][42]	= settr(2037,2,68,22,22,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 44 -> 68,0 */
	reached9[44] = 1;
	trans[9][44]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][45]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][46]	= settr(2041,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][51]	= settr(2046,2,68,1,0,".(goto)", 1, 2500, 0);
	trans[9][47]	= settr(2042,2,48,2,0,"else", 1, 2500, 0);
	trans[9][48]	= settr(2043,2,68,23,23,"transition_id = -(1)", 1, 2500, 0);
	trans[9][49]	= settr(2044,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][52]	= settr(2047,2,54,24,24,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][54] = settr(2049,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2049,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][53]	= settr(2048,2,64,1,0,"(1)", 1, 2500, 0);
	T = trans[9][64] = settr(2059,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2059,2,55,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2059,2,61,0,0,"IF", 1, 2500, 0);
	trans[9][55]	= settr(2050,2,57,25,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 9][57] = settr(2052,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2052,0,56,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][56]	= settr(2051,2,68,26,26,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 58 -> 68,0 */
	reached9[58] = 1;
	trans[9][58]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][59]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[9][60]	= settr(2055,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][65]	= settr(2060,2,68,1,0,".(goto)", 1, 2500, 0);
	trans[9][61]	= settr(2056,2,62,2,0,"else", 1, 2500, 0);
	trans[9][62]	= settr(2057,2,68,27,27,"transition_id = -(1)", 1, 2500, 0);
	trans[9][63]	= settr(2058,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][68]	= settr(2063,2,69,1,0,"(1)", 1, 2500, 0);
	trans[9][71]	= settr(2066,2,74,1,0,"break", 1, 2500, 0);
	trans[9][74]	= settr(2069,2,142,28,28,"Actuator1_lock!1", 1, 2500, 0);
	T = trans[ 9][142] = settr(2137,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2137,0,78,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][78] = settr(2073,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2073,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][75]	= settr(2070,2,77,29,29,"global_state.actuator2.sender = 6", 1, 2500, 0);
	T = trans[ 9][77] = settr(2072,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2072,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][76]	= settr(2071,2,141,30,0,"assert((((((((global_state.actuator2.sender==0)||(global_state.actuator2.sender==1))||(global_state.actuator2.sender==2))||(global_state.actuator2.sender==3))||(global_state.actuator2.sender==4))||(global_state.actuator2.sender==5))||(global_state.actuator2.sender==6)))", 1, 2500, 0);
	T = trans[ 9][141] = settr(2136,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2136,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][79]	= settr(2074,2,138,31,31,"transition_id = 0", 1, 2500, 0); /* m: 80 -> 0,138 */
	reached9[80] = 1;
	trans[9][80]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][139]	= settr(2134,2,138,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][138] = settr(2133,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2133,2,135,0,0,"DO", 1, 2500, 0);
	T = trans[9][135] = settr(2130,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2130,2,81,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2130,2,83,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2130,2,87,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2130,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2130,2,121,0,0,"IF", 1, 2500, 0);
	trans[9][81]	= settr(2076,2,143,32,32,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][82]	= settr(2077,2,143,1,0,"goto :b38", 1, 2500, 0);
	trans[9][136]	= settr(2131,2,137,1,0,".(goto)", 1, 2500, 0);
	trans[9][83]	= settr(2078,2,137,33,33,"((transition_id==0))", 1, 2500, 0); /* m: 84 -> 137,0 */
	reached9[84] = 1;
	trans[9][84]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][85]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][86]	= settr(2081,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][87]	= settr(2082,2,89,34,34,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][89] = settr(2084,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2084,0,88,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][88]	= settr(2083,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[9][105] = settr(2100,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2100,2,90,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2100,2,96,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2100,2,102,0,0,"IF", 1, 2500, 0);
	trans[9][90]	= settr(2085,2,92,35,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 9][92] = settr(2087,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2087,0,91,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][91]	= settr(2086,2,137,36,36,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 93 -> 137,0 */
	reached9[93] = 1;
	trans[9][93]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][94]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][95]	= settr(2090,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][106]	= settr(2101,2,137,1,0,".(goto)", 1, 2500, 0);
	trans[9][96]	= settr(2091,2,98,37,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 9][98] = settr(2093,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2093,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][97]	= settr(2092,2,137,38,38,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 99 -> 137,0 */
	reached9[99] = 1;
	trans[9][99]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][100]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[9][101]	= settr(2096,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][102]	= settr(2097,2,103,2,0,"else", 1, 2500, 0);
	trans[9][103]	= settr(2098,2,137,39,39,"transition_id = -(1)", 1, 2500, 0);
	trans[9][104]	= settr(2099,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][107]	= settr(2102,2,109,40,40,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][109] = settr(2104,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2104,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][108]	= settr(2103,2,119,1,0,"(1)", 1, 2500, 0);
	T = trans[9][119] = settr(2114,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2114,2,110,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2114,2,116,0,0,"IF", 1, 2500, 0);
	trans[9][110]	= settr(2105,2,112,41,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 9][112] = settr(2107,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2107,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][111]	= settr(2106,2,137,42,42,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 113 -> 137,0 */
	reached9[113] = 1;
	trans[9][113]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][114]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][115]	= settr(2110,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][120]	= settr(2115,2,137,1,0,".(goto)", 1, 2500, 0);
	trans[9][116]	= settr(2111,2,117,2,0,"else", 1, 2500, 0);
	trans[9][117]	= settr(2112,2,137,43,43,"transition_id = -(1)", 1, 2500, 0);
	trans[9][118]	= settr(2113,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][121]	= settr(2116,2,123,44,44,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][123] = settr(2118,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2118,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][122]	= settr(2117,2,133,1,0,"(1)", 1, 2500, 0);
	T = trans[9][133] = settr(2128,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2128,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2128,2,130,0,0,"IF", 1, 2500, 0);
	trans[9][124]	= settr(2119,2,126,45,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 9][126] = settr(2121,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2121,0,125,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][125]	= settr(2120,2,137,46,46,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 127 -> 137,0 */
	reached9[127] = 1;
	trans[9][127]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][128]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[9][129]	= settr(2124,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][134]	= settr(2129,2,137,1,0,".(goto)", 1, 2500, 0);
	trans[9][130]	= settr(2125,2,131,2,0,"else", 1, 2500, 0);
	trans[9][131]	= settr(2126,2,137,47,47,"transition_id = -(1)", 1, 2500, 0);
	trans[9][132]	= settr(2127,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][137]	= settr(2132,2,138,1,0,"(1)", 1, 2500, 0);
	trans[9][140]	= settr(2135,2,143,1,0,"break", 1, 2500, 0);
	trans[9][143]	= settr(2138,2,292,48,48,"Actuator2_lock!1", 1, 2500, 0);
	T = trans[ 9][292] = settr(2287,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2287,0,147,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][147] = settr(2142,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2142,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][144]	= settr(2139,2,146,49,49,"global_state.modemanager.sender = 6", 1, 2500, 0);
	T = trans[ 9][146] = settr(2141,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2141,0,145,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][145]	= settr(2140,2,291,50,0,"assert((((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4))||(global_state.modemanager.sender==5))||(global_state.modemanager.sender==6)))", 1, 2500, 0);
	T = trans[ 9][291] = settr(2286,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2286,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][148]	= settr(2143,2,288,51,51,"transition_id = 0", 1, 2500, 0); /* m: 149 -> 0,288 */
	reached9[149] = 1;
	trans[9][149]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][289]	= settr(2284,2,288,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][288] = settr(2283,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2283,2,285,0,0,"DO", 1, 2500, 0);
	T = trans[9][285] = settr(2280,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2280,2,150,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2280,2,152,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2280,2,156,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2280,2,198,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2280,2,232,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2280,2,253,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2280,2,257,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2280,2,269,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2280,2,273,0,0,"IF", 1, 2500, 0);
	trans[9][150]	= settr(2145,2,293,52,52,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][151]	= settr(2146,2,293,1,0,"goto :b39", 1, 2500, 0);
	trans[9][286]	= settr(2281,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][152]	= settr(2147,2,287,53,53,"((transition_id==0))", 1, 2500, 0); /* m: 153 -> 287,0 */
	reached9[153] = 1;
	trans[9][153]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][154]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[9][155]	= settr(2150,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][156]	= settr(2151,2,158,54,54,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][158] = settr(2153,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2153,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][157]	= settr(2152,2,196,1,0,"(1)", 1, 2500, 0);
	T = trans[9][196] = settr(2191,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2191,2,159,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2191,2,175,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2191,2,193,0,0,"IF", 1, 2500, 0);
	trans[9][159]	= settr(2154,2,163,55,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 9][163] = settr(2158,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2158,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][160]	= settr(2155,2,162,56,56,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 9][162] = settr(2157,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2157,0,161,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][161]	= settr(2156,2,167,57,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 9][167] = settr(2162,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2162,0,164,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][164]	= settr(2159,2,166,58,58,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 9][166] = settr(2161,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2161,0,165,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][165]	= settr(2160,2,169,59,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 9][169] = settr(2164,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2164,0,168,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][168]	= settr(2163,2,171,60,60,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 9][171] = settr(2166,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2166,0,170,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][170]	= settr(2165,2,287,61,61,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 172 -> 287,0 */
	reached9[172] = 1;
	trans[9][172]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][173]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[9][174]	= settr(2169,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][197]	= settr(2192,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][175]	= settr(2170,2,188,62,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 9][188] = settr(2183,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2183,0,179,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][179] = settr(2174,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2174,0,176,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][176]	= settr(2171,2,178,63,63,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 9][178] = settr(2173,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2173,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][177]	= settr(2172,2,183,64,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 9][183] = settr(2178,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2178,0,180,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][180]	= settr(2175,2,182,65,65,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 9][182] = settr(2177,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2177,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][181]	= settr(2176,2,187,66,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 9][187] = settr(2182,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2182,0,184,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][184]	= settr(2179,2,186,67,67,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 9][186] = settr(2181,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2181,0,185,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][185]	= settr(2180,2,190,68,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 9][190] = settr(2185,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2185,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][189]	= settr(2184,2,287,69,69,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 191 -> 287,0 */
	reached9[191] = 1;
	trans[9][191]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][192]	= settr(2187,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][193]	= settr(2188,2,194,2,0,"else", 1, 2500, 0);
	trans[9][194]	= settr(2189,2,287,70,70,"transition_id = -(1)", 1, 2500, 0);
	trans[9][195]	= settr(2190,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][198]	= settr(2193,2,200,71,71,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][200] = settr(2195,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2195,0,199,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][199]	= settr(2194,2,230,1,0,"(1)", 1, 2500, 0);
	T = trans[9][230] = settr(2225,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2225,2,201,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2225,2,209,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2225,2,227,0,0,"IF", 1, 2500, 0);
	trans[9][201]	= settr(2196,2,203,72,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 9][203] = settr(2198,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2198,0,202,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][202]	= settr(2197,2,205,73,73,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 9][205] = settr(2200,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2200,0,204,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][204]	= settr(2199,2,287,74,74,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 206 -> 287,0 */
	reached9[206] = 1;
	trans[9][206]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][207]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[9][208]	= settr(2203,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][231]	= settr(2226,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][209]	= settr(2204,2,222,75,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 9][222] = settr(2217,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2217,0,213,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][213] = settr(2208,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2208,0,210,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][210]	= settr(2205,2,212,76,76,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 9][212] = settr(2207,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2207,0,211,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][211]	= settr(2206,2,217,77,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 9][217] = settr(2212,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2212,0,214,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][214]	= settr(2209,2,216,78,78,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 9][216] = settr(2211,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2211,0,215,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][215]	= settr(2210,2,221,79,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 9][221] = settr(2216,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2216,0,218,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][218]	= settr(2213,2,220,80,80,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 9][220] = settr(2215,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2215,0,219,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][219]	= settr(2214,2,224,81,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 9][224] = settr(2219,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2219,0,223,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][223]	= settr(2218,2,287,82,82,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 225 -> 287,0 */
	reached9[225] = 1;
	trans[9][225]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][226]	= settr(2221,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][227]	= settr(2222,2,228,2,0,"else", 1, 2500, 0);
	trans[9][228]	= settr(2223,2,287,83,83,"transition_id = -(1)", 1, 2500, 0);
	trans[9][229]	= settr(2224,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][232]	= settr(2227,2,234,84,84,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][234] = settr(2229,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2229,0,233,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][233]	= settr(2228,2,241,1,0,"(1)", 1, 2500, 0);
	T = trans[9][241] = settr(2236,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2236,2,235,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2236,2,238,0,0,"IF", 1, 2500, 0);
	trans[9][235]	= settr(2230,2,237,85,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 9][237] = settr(2232,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2232,0,236,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][236]	= settr(2231,2,249,86,86,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[9][242]	= settr(2237,2,249,1,0,".(goto)", 1, 2500, 0);
	trans[9][238]	= settr(2233,2,240,2,0,"else", 1, 2500, 0);
	T = trans[ 9][240] = settr(2235,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2235,0,239,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][239]	= settr(2234,2,249,87,87,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[9][249] = settr(2244,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2244,2,243,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2244,2,246,0,0,"IF", 1, 2500, 0);
	trans[9][243]	= settr(2238,2,245,88,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 9][245] = settr(2240,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2240,0,244,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][244]	= settr(2239,2,287,89,89,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 251 -> 287,0 */
	reached9[251] = 1;
	trans[9][250]	= settr(2245,2,251,1,0,".(goto)", 1, 2500, 0); /* m: 251 -> 0,287 */
	reached9[251] = 1;
	trans[9][246]	= settr(2241,2,248,2,0,"else", 1, 2500, 0);
	T = trans[ 9][248] = settr(2243,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2243,0,247,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][247]	= settr(2242,2,287,90,90,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 251 -> 287,0 */
	reached9[251] = 1;
	trans[9][251]	= settr(2246,2,287,91,91,"transition_id = -(1)", 1, 2500, 0);
	trans[9][252]	= settr(2247,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][253]	= settr(2248,2,255,92,92,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 9][255] = settr(2250,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2250,0,254,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][254]	= settr(2249,2,267,1,0,"(1)", 1, 2500, 0);
	trans[9][256]	= settr(2251,2,267,1,0,"goto startcheck", 1, 2500, 0);
	trans[9][257]	= settr(2252,2,259,93,93,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 9][259] = settr(2254,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2254,0,258,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][258]	= settr(2253,2,267,1,0,"(1)", 1, 2500, 0);
	T = trans[9][267] = settr(2262,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2262,2,260,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2262,2,263,0,0,"IF", 1, 2500, 0);
	trans[9][260]	= settr(2255,2,287,94,94,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 261 -> 287,0 */
	reached9[261] = 1;
	trans[9][261]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][262]	= settr(2257,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][268]	= settr(2263,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][263]	= settr(2258,2,287,95,95,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 264 -> 287,0 */
	reached9[264] = 1;
	trans[9][264]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][265]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[9][266]	= settr(2261,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][269]	= settr(2264,2,271,96,96,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 9][271] = settr(2266,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2266,0,270,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][270]	= settr(2265,2,283,1,0,"(1)", 1, 2500, 0);
	trans[9][272]	= settr(2267,2,283,1,0,"goto stopcheck", 1, 2500, 0);
	trans[9][273]	= settr(2268,2,275,97,97,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 9][275] = settr(2270,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2270,0,274,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][274]	= settr(2269,2,283,1,0,"(1)", 1, 2500, 0);
	T = trans[9][283] = settr(2278,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2278,2,276,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2278,2,279,0,0,"IF", 1, 2500, 0);
	trans[9][276]	= settr(2271,2,287,98,98,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 277 -> 287,0 */
	reached9[277] = 1;
	trans[9][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][278]	= settr(2273,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][284]	= settr(2279,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][279]	= settr(2274,2,287,99,99,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 280 -> 287,0 */
	reached9[280] = 1;
	trans[9][280]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][281]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[9][282]	= settr(2277,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][287]	= settr(2282,2,288,1,0,"(1)", 1, 2500, 0);
	trans[9][290]	= settr(2285,2,293,1,0,"break", 1, 2500, 0);
	trans[9][293]	= settr(2288,2,337,100,100,"Modemanager_lock!1", 1, 2500, 0);
	T = trans[ 9][337] = settr(2332,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2332,0,297,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][297] = settr(2292,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2292,0,294,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][294]	= settr(2289,2,296,101,101,"global_state.untitled_msc.sender = 6", 1, 2500, 0);
	T = trans[ 9][296] = settr(2291,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2291,0,295,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][295]	= settr(2290,2,336,102,0,"assert((((((((global_state.untitled_msc.sender==0)||(global_state.untitled_msc.sender==1))||(global_state.untitled_msc.sender==2))||(global_state.untitled_msc.sender==3))||(global_state.untitled_msc.sender==4))||(global_state.untitled_msc.sender==5))||(global_state.untitled_msc.sender==6)))", 1, 2500, 0);
	T = trans[ 9][336] = settr(2331,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2331,0,298,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][298]	= settr(2293,2,333,103,103,"transition_id = 0", 1, 2500, 0); /* m: 299 -> 0,333 */
	reached9[299] = 1;
	trans[9][299]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][334]	= settr(2329,2,333,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][333] = settr(2328,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2328,2,330,0,0,"DO", 1, 2500, 0);
	T = trans[9][330] = settr(2325,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2325,2,300,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2325,2,302,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2325,2,306,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2325,2,318,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2325,2,322,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2325,2,326,0,0,"IF", 1, 2500, 0);
	trans[9][300]	= settr(2295,2,338,104,104,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][301]	= settr(2296,2,338,1,0,"goto :b40", 1, 2500, 0);
	trans[9][331]	= settr(2326,2,332,1,0,".(goto)", 1, 2500, 0);
	trans[9][302]	= settr(2297,2,332,105,105,"((transition_id==0))", 1, 2500, 0); /* m: 303 -> 332,0 */
	reached9[303] = 1;
	trans[9][303]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][304]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[9][305]	= settr(2300,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][306]	= settr(2301,2,313,106,106,"((transition_id==1))", 1, 2500, 0);
	T = trans[9][313] = settr(2308,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2308,2,307,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2308,2,311,0,0,"IF", 1, 2500, 0);
	trans[9][307]	= settr(2302,2,332,107,107,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 308 -> 332,0 */
	reached9[308] = 1;
	trans[9][308]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][309]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[9][310]	= settr(2305,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][314]	= settr(2309,2,315,1,0,".(goto)", 1, 2500, 0); /* m: 315 -> 0,332 */
	reached9[315] = 1;
	trans[9][311]	= settr(2306,2,312,2,0,"else", 1, 2500, 0);
	trans[9][312]	= settr(2307,2,332,108,108,"(1)", 1, 2500, 0); /* m: 315 -> 332,0 */
	reached9[315] = 1;
	trans[9][315]	= settr(2310,2,332,109,109,"transition_id = -(1)", 1, 2500, 0); /* m: 316 -> 0,332 */
	reached9[316] = 1;
	trans[9][316]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[9][317]	= settr(2312,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][318]	= settr(2313,2,332,110,110,"((transition_id==2))", 1, 2500, 0); /* m: 319 -> 332,0 */
	reached9[319] = 1;
	trans[9][319]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][320]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[9][321]	= settr(2316,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][322]	= settr(2317,2,332,111,111,"((transition_id==3))", 1, 2500, 0); /* m: 323 -> 332,0 */
	reached9[323] = 1;
	trans[9][323]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][324]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[9][325]	= settr(2320,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][326]	= settr(2321,2,332,112,112,"((transition_id==4))", 1, 2500, 0); /* m: 327 -> 332,0 */
	reached9[327] = 1;
	trans[9][327]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][328]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[9][329]	= settr(2324,2,332,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][332]	= settr(2327,2,333,1,0,"(1)", 1, 2500, 0);
	trans[9][335]	= settr(2330,2,338,1,0,"break", 1, 2500, 0);
	trans[9][338]	= settr(2333,4,341,113,113,"Untitled_msc_lock!1", 1, 2500, 0); /* m: 339 -> 341,0 */
	reached9[339] = 1;
	trans[9][339]	= settr(0,0,0,0,0,"inited = 1",0,0,0);
	trans[9][341]	= settr(2336,0,0,114,114,"-end-", 0, 3500, 0);

	/* proctype 8: Sensor_reading */

	trans[8] = (Trans **) emalloc(39*sizeof(Trans *));

	trans[8][1]	= settr(1958,0,2,115,0,"(inited)", 1, 2, 0);
	trans[8][2]	= settr(1959,0,35,116,116,"value = 0", 0, 2, 0);
	trans[8][36]	= settr(1993,0,35,1,0,".(goto)", 0, 2, 0);
	T = trans[8][35] = settr(1992,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1992,0,34,0,0,"DO", 0, 2, 0);
	T = trans[ 8][34] = settr(1991,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1991,2,31,0,0,"ATOMIC", 1, 2, 0);
	T = trans[ 8][31] = settr(1988,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1988,0,3,0,0,"sub-sequence", 1, 2, 0);
	trans[8][3]	= settr(1960,2,28,117,117,"Sensor_reading_x_tmp = 0", 1, 2, 0);
	T = trans[8][28] = settr(1985,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(1985,2,11,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(1985,2,19,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(1985,2,27,0,0,"IF", 1, 2, 0);
	T = trans[ 8][11] = settr(1968,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1968,0,4,0,0,"sub-sequence", 1, 2, 0);
	trans[8][4]	= settr(1961,2,8,118,118,"Sensor_reading_x_tmp = 10", 1, 2, 0);
	trans[8][9]	= settr(1966,2,8,1,0,".(goto)", 1, 2, 0);
	T = trans[8][8] = settr(1965,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(1965,2,7,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(1965,2,5,0,0,"DO", 1, 2, 0);
	trans[8][7]	= settr(1964,2,33,119,119,"goto :b34", 1, 2, 0); /* m: 30 -> 0,33 */
	reached8[30] = 1;
	trans[8][5]	= settr(1962,2,8,120,120,"((Sensor_reading_x_tmp<10))", 1, 2, 0); /* m: 6 -> 8,0 */
	reached8[6] = 1;
	trans[8][6]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[8][10]	= settr(1967,2,30,1,0,"break", 1, 2, 0);
	trans[8][29]	= settr(1986,2,30,1,0,".(goto)", 1, 2, 0); /* m: 30 -> 0,33 */
	reached8[30] = 1;
	T = trans[ 8][19] = settr(1976,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1976,0,12,0,0,"sub-sequence", 1, 2, 0);
	trans[8][12]	= settr(1969,2,16,121,121,"Sensor_reading_x_tmp = 50", 1, 2, 0);
	trans[8][17]	= settr(1974,2,16,1,0,".(goto)", 1, 2, 0);
	T = trans[8][16] = settr(1973,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(1973,2,15,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(1973,2,13,0,0,"DO", 1, 2, 0);
	trans[8][15]	= settr(1972,2,33,122,122,"goto :b35", 1, 2, 0); /* m: 30 -> 0,33 */
	reached8[30] = 1;
	trans[8][13]	= settr(1970,2,16,123,123,"((Sensor_reading_x_tmp<50))", 1, 2, 0); /* m: 14 -> 16,0 */
	reached8[14] = 1;
	trans[8][14]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[8][18]	= settr(1975,2,30,1,0,"break", 1, 2, 0);
	T = trans[ 8][27] = settr(1984,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1984,0,20,0,0,"sub-sequence", 1, 2, 0);
	trans[8][20]	= settr(1977,2,24,124,124,"Sensor_reading_x_tmp = 100", 1, 2, 0);
	trans[8][25]	= settr(1982,2,24,1,0,".(goto)", 1, 2, 0);
	T = trans[8][24] = settr(1981,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(1981,2,23,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(1981,2,21,0,0,"DO", 1, 2, 0);
	trans[8][23]	= settr(1980,2,33,125,125,"goto :b36", 1, 2, 0); /* m: 30 -> 0,33 */
	reached8[30] = 1;
	trans[8][21]	= settr(1978,2,24,126,126,"((Sensor_reading_x_tmp<100))", 1, 2, 0); /* m: 22 -> 24,0 */
	reached8[22] = 1;
	trans[8][22]	= settr(0,0,0,0,0,"Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)",0,0,0);
	trans[8][26]	= settr(1983,2,30,1,0,"break", 1, 2, 0);
	trans[8][30]	= settr(1987,2,33,127,127,"value = Sensor_reading_x_tmp", 1, 2, 0);
	T = trans[ 8][33] = settr(1990,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1990,0,32,0,0,"sub-sequence", 1, 2, 0);
	trans[8][32]	= settr(1989,0,35,128,128,"Modemanager_reading_channel!value", 1, 8, 0);
	trans[8][37]	= settr(1994,0,38,1,0,"break", 0, 2, 0);
	trans[8][38]	= settr(1995,0,0,129,129,"-end-", 0, 3500, 0);

	/* proctype 7: Modemanager_systemTc */

	trans[7] = (Trans **) emalloc(478*sizeof(Trans *));

	trans[7][1]	= settr(1481,0,474,130,0,"(inited)", 1, 2, 0);
	trans[7][475]	= settr(1955,0,474,1,0,".(goto)", 0, 2, 0);
	T = trans[7][474] = settr(1954,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1954,0,473,0,0,"DO", 0, 2, 0);
	T = trans[ 7][473] = settr(1953,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1953,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[7][2]	= settr(1482,2,3,131,0,"(nempty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][3]	= settr(1483,2,470,132,132,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[7][470] = settr(1950,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1950,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1950,2,468,0,0,"IF", 1, 2500, 0);
	trans[7][4]	= settr(1484,2,5,133,0,"(nempty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][5]	= settr(1485,2,353,134,134,"Modemanager_systemTc_channel?Modemanager_systemtc_signal_parameter.data.goToIdle,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2,Modemanager_systemtc_signal_parameter.data.doReport,Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0); /* m: 6 -> 353,0 */
	reached7[6] = 1;
	trans[7][6]	= settr(0,0,0,0,0,"Modemanager_systemtc_channel_used = 1",0,0,0);
	T = trans[ 7][353] = settr(1833,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1833,0,351,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[7][351] = settr(1831,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1831,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1831,2,178,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1831,2,349,0,0,"IF", 1, 2500, 0);
	trans[7][7]	= settr(1487,2,33,135,0,"((global_state.modemanager.state==3))", 1, 2500, 0);
	T = trans[ 7][33] = settr(1513,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1513,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][8]	= settr(1488,2,31,136,136,"global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0);
	T = trans[7][31] = settr(1511,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1511,2,9,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1511,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1511,2,24,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1511,2,29,0,0,"IF", 1, 2500, 0);
	trans[7][9]	= settr(1489,2,13,137,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][13] = settr(1493,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1493,0,10,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][10]	= settr(1490,2,12,138,138,"global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle", 1, 2500, 0);
	T = trans[ 7][12] = settr(1492,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1492,0,11,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][11]	= settr(1491,2,177,139,0,"assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))", 1, 2500, 0);
	trans[7][32]	= settr(1512,2,177,1,0,".(goto)", 1, 2500, 0);
	trans[7][14]	= settr(1494,2,23,140,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][23] = settr(1503,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1503,0,18,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][18] = settr(1498,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1498,0,15,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][15]	= settr(1495,2,17,141,141,"global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][17] = settr(1497,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1497,0,16,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][16]	= settr(1496,2,22,142,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][22] = settr(1502,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1502,0,19,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][19]	= settr(1499,2,21,143,143,"global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][21] = settr(1501,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1501,0,20,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][20]	= settr(1500,2,177,144,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))", 1, 2500, 0);
	trans[7][24]	= settr(1504,2,28,145,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][28] = settr(1508,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1508,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][25]	= settr(1505,2,27,146,146,"global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport", 1, 2500, 0);
	T = trans[ 7][27] = settr(1507,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1507,0,26,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][26]	= settr(1506,2,177,147,0,"assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))", 1, 2500, 0);
	trans[7][29]	= settr(1509,2,30,2,0,"else", 1, 2500, 0);
	trans[7][30]	= settr(1510,2,177,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][177] = settr(1657,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1657,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][34]	= settr(1514,2,174,148,148,"transition_id = 0", 1, 2500, 0); /* m: 35 -> 0,174 */
	reached7[35] = 1;
	trans[7][35]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[7][175]	= settr(1655,2,174,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][174] = settr(1654,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1654,2,171,0,0,"DO", 1, 2500, 0);
	T = trans[7][171] = settr(1651,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1651,2,36,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1651,2,38,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1651,2,42,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1651,2,84,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1651,2,118,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1651,2,139,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1651,2,143,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1651,2,155,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1651,2,159,0,0,"IF", 1, 2500, 0);
	trans[7][36]	= settr(1516,2,354,149,149,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][37]	= settr(1517,2,354,1,0,"goto :b29", 1, 2500, 0);
	trans[7][172]	= settr(1652,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][38]	= settr(1518,2,173,150,150,"((transition_id==0))", 1, 2500, 0); /* m: 39 -> 173,0 */
	reached7[39] = 1;
	trans[7][39]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][40]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][41]	= settr(1521,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][42]	= settr(1522,2,44,151,151,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][44] = settr(1524,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1524,0,43,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][43]	= settr(1523,2,82,1,0,"(1)", 1, 2500, 0);
	T = trans[7][82] = settr(1562,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1562,2,45,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1562,2,61,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1562,2,79,0,0,"IF", 1, 2500, 0);
	trans[7][45]	= settr(1525,2,49,152,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][49] = settr(1529,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1529,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][46]	= settr(1526,2,48,153,153,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][48] = settr(1528,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1528,0,47,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][47]	= settr(1527,2,53,154,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][53] = settr(1533,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1533,0,50,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][50]	= settr(1530,2,52,155,155,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][52] = settr(1532,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1532,0,51,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][51]	= settr(1531,2,55,156,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 7][55] = settr(1535,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1535,0,54,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][54]	= settr(1534,2,57,157,157,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 7][57] = settr(1537,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1537,0,56,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][56]	= settr(1536,2,173,158,158,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 58 -> 173,0 */
	reached7[58] = 1;
	trans[7][58]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][59]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[7][60]	= settr(1540,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][83]	= settr(1563,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][61]	= settr(1541,2,74,159,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][74] = settr(1554,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1554,0,65,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][65] = settr(1545,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1545,0,62,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][62]	= settr(1542,2,64,160,160,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][64] = settr(1544,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1544,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][63]	= settr(1543,2,69,161,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][69] = settr(1549,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1549,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][66]	= settr(1546,2,68,162,162,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][68] = settr(1548,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1548,0,67,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][67]	= settr(1547,2,73,163,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][73] = settr(1553,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1553,0,70,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][70]	= settr(1550,2,72,164,164,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][72] = settr(1552,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1552,0,71,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][71]	= settr(1551,2,76,165,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][76] = settr(1556,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1556,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][75]	= settr(1555,2,173,166,166,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 77 -> 173,0 */
	reached7[77] = 1;
	trans[7][77]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][78]	= settr(1558,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][79]	= settr(1559,2,80,2,0,"else", 1, 2500, 0);
	trans[7][80]	= settr(1560,2,173,167,167,"transition_id = -(1)", 1, 2500, 0);
	trans[7][81]	= settr(1561,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][84]	= settr(1564,2,86,168,168,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 7][86] = settr(1566,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1566,0,85,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][85]	= settr(1565,2,116,1,0,"(1)", 1, 2500, 0);
	T = trans[7][116] = settr(1596,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1596,2,87,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1596,2,95,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1596,2,113,0,0,"IF", 1, 2500, 0);
	trans[7][87]	= settr(1567,2,89,169,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][89] = settr(1569,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1569,0,88,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][88]	= settr(1568,2,91,170,170,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 7][91] = settr(1571,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1571,0,90,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][90]	= settr(1570,2,173,171,171,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 92 -> 173,0 */
	reached7[92] = 1;
	trans[7][92]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][93]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[7][94]	= settr(1574,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][117]	= settr(1597,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][95]	= settr(1575,2,108,172,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][108] = settr(1588,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1588,0,99,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][99] = settr(1579,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1579,0,96,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][96]	= settr(1576,2,98,173,173,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][98] = settr(1578,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1578,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][97]	= settr(1577,2,103,174,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][103] = settr(1583,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1583,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][100]	= settr(1580,2,102,175,175,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][102] = settr(1582,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1582,0,101,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][101]	= settr(1581,2,107,176,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][107] = settr(1587,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1587,0,104,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][104]	= settr(1584,2,106,177,177,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][106] = settr(1586,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1586,0,105,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][105]	= settr(1585,2,110,178,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][110] = settr(1590,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1590,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][109]	= settr(1589,2,173,179,179,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 111 -> 173,0 */
	reached7[111] = 1;
	trans[7][111]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][112]	= settr(1592,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][113]	= settr(1593,2,114,2,0,"else", 1, 2500, 0);
	trans[7][114]	= settr(1594,2,173,180,180,"transition_id = -(1)", 1, 2500, 0);
	trans[7][115]	= settr(1595,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][118]	= settr(1598,2,120,181,181,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 7][120] = settr(1600,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1600,0,119,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][119]	= settr(1599,2,127,1,0,"(1)", 1, 2500, 0);
	T = trans[7][127] = settr(1607,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1607,2,121,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1607,2,124,0,0,"IF", 1, 2500, 0);
	trans[7][121]	= settr(1601,2,123,182,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 7][123] = settr(1603,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1603,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][122]	= settr(1602,2,135,183,183,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[7][128]	= settr(1608,2,135,1,0,".(goto)", 1, 2500, 0);
	trans[7][124]	= settr(1604,2,126,2,0,"else", 1, 2500, 0);
	T = trans[ 7][126] = settr(1606,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1606,0,125,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][125]	= settr(1605,2,135,184,184,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[7][135] = settr(1615,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1615,2,129,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1615,2,132,0,0,"IF", 1, 2500, 0);
	trans[7][129]	= settr(1609,2,131,185,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 7][131] = settr(1611,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1611,0,130,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][130]	= settr(1610,2,173,186,186,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 137 -> 173,0 */
	reached7[137] = 1;
	trans[7][136]	= settr(1616,2,137,1,0,".(goto)", 1, 2500, 0); /* m: 137 -> 0,173 */
	reached7[137] = 1;
	trans[7][132]	= settr(1612,2,134,2,0,"else", 1, 2500, 0);
	T = trans[ 7][134] = settr(1614,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1614,0,133,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][133]	= settr(1613,2,173,187,187,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 137 -> 173,0 */
	reached7[137] = 1;
	trans[7][137]	= settr(1617,2,173,188,188,"transition_id = -(1)", 1, 2500, 0);
	trans[7][138]	= settr(1618,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][139]	= settr(1619,2,141,189,189,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 7][141] = settr(1621,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1621,0,140,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][140]	= settr(1620,2,153,1,0,"(1)", 1, 2500, 0);
	trans[7][142]	= settr(1622,2,153,1,0,"goto startcheck", 1, 2500, 0);
	trans[7][143]	= settr(1623,2,145,190,190,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 7][145] = settr(1625,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1625,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][144]	= settr(1624,2,153,1,0,"(1)", 1, 2500, 0);
	T = trans[7][153] = settr(1633,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1633,2,146,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1633,2,149,0,0,"IF", 1, 2500, 0);
	trans[7][146]	= settr(1626,2,173,191,191,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 147 -> 173,0 */
	reached7[147] = 1;
	trans[7][147]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][148]	= settr(1628,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][154]	= settr(1634,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][149]	= settr(1629,2,173,192,192,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 150 -> 173,0 */
	reached7[150] = 1;
	trans[7][150]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][151]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[7][152]	= settr(1632,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][155]	= settr(1635,2,157,193,193,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 7][157] = settr(1637,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1637,0,156,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][156]	= settr(1636,2,169,1,0,"(1)", 1, 2500, 0);
	trans[7][158]	= settr(1638,2,169,1,0,"goto stopcheck", 1, 2500, 0);
	trans[7][159]	= settr(1639,2,161,194,194,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 7][161] = settr(1641,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1641,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][160]	= settr(1640,2,169,1,0,"(1)", 1, 2500, 0);
	T = trans[7][169] = settr(1649,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1649,2,162,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1649,2,165,0,0,"IF", 1, 2500, 0);
	trans[7][162]	= settr(1642,2,173,195,195,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 163 -> 173,0 */
	reached7[163] = 1;
	trans[7][163]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][164]	= settr(1644,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][170]	= settr(1650,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][165]	= settr(1645,2,173,196,196,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 166 -> 173,0 */
	reached7[166] = 1;
	trans[7][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][168]	= settr(1648,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][173]	= settr(1653,2,174,1,0,"(1)", 1, 2500, 0);
	trans[7][176]	= settr(1656,2,354,1,0,"break", 1, 2500, 0);
	trans[7][352]	= settr(1832,2,354,1,0,".(goto)", 1, 2500, 0);
	trans[7][178]	= settr(1658,2,204,197,0,"((global_state.modemanager.state==1))", 1, 2500, 0);
	T = trans[ 7][204] = settr(1684,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1684,0,179,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][179]	= settr(1659,2,202,198,198,"global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0);
	T = trans[7][202] = settr(1682,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1682,2,180,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1682,2,185,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1682,2,195,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1682,2,200,0,0,"IF", 1, 2500, 0);
	trans[7][180]	= settr(1660,2,184,199,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][184] = settr(1664,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1664,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][181]	= settr(1661,2,183,200,200,"global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle", 1, 2500, 0);
	T = trans[ 7][183] = settr(1663,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1663,0,182,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][182]	= settr(1662,2,348,201,0,"assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))", 1, 2500, 0);
	trans[7][203]	= settr(1683,2,348,1,0,".(goto)", 1, 2500, 0);
	trans[7][185]	= settr(1665,2,194,202,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][194] = settr(1674,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1674,0,189,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][189] = settr(1669,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1669,0,186,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][186]	= settr(1666,2,188,203,203,"global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][188] = settr(1668,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1668,0,187,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][187]	= settr(1667,2,193,204,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][193] = settr(1673,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1673,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][190]	= settr(1670,2,192,205,205,"global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][192] = settr(1672,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1672,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][191]	= settr(1671,2,348,206,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))", 1, 2500, 0);
	trans[7][195]	= settr(1675,2,199,207,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][199] = settr(1679,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1679,0,196,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][196]	= settr(1676,2,198,208,208,"global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport", 1, 2500, 0);
	T = trans[ 7][198] = settr(1678,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1678,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][197]	= settr(1677,2,348,209,0,"assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))", 1, 2500, 0);
	trans[7][200]	= settr(1680,2,201,2,0,"else", 1, 2500, 0);
	trans[7][201]	= settr(1681,2,348,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][348] = settr(1828,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1828,0,205,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][205]	= settr(1685,2,345,210,210,"transition_id = 0", 1, 2500, 0); /* m: 206 -> 0,345 */
	reached7[206] = 1;
	trans[7][206]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[7][346]	= settr(1826,2,345,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][345] = settr(1825,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1825,2,342,0,0,"DO", 1, 2500, 0);
	T = trans[7][342] = settr(1822,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1822,2,207,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1822,2,209,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1822,2,213,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1822,2,255,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1822,2,289,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1822,2,310,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1822,2,314,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1822,2,326,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1822,2,330,0,0,"IF", 1, 2500, 0);
	trans[7][207]	= settr(1687,2,354,211,211,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][208]	= settr(1688,2,354,1,0,"goto :b30", 1, 2500, 0);
	trans[7][343]	= settr(1823,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][209]	= settr(1689,2,344,212,212,"((transition_id==0))", 1, 2500, 0); /* m: 210 -> 344,0 */
	reached7[210] = 1;
	trans[7][210]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][211]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][212]	= settr(1692,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][213]	= settr(1693,2,215,213,213,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][215] = settr(1695,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1695,0,214,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][214]	= settr(1694,2,253,1,0,"(1)", 1, 2500, 0);
	T = trans[7][253] = settr(1733,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1733,2,216,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1733,2,232,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1733,2,250,0,0,"IF", 1, 2500, 0);
	trans[7][216]	= settr(1696,2,220,214,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][220] = settr(1700,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1700,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][217]	= settr(1697,2,219,215,215,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][219] = settr(1699,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1699,0,218,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][218]	= settr(1698,2,224,216,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][224] = settr(1704,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1704,0,221,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][221]	= settr(1701,2,223,217,217,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][223] = settr(1703,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1703,0,222,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][222]	= settr(1702,2,226,218,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 7][226] = settr(1706,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1706,0,225,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][225]	= settr(1705,2,228,219,219,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 7][228] = settr(1708,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1708,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][227]	= settr(1707,2,344,220,220,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 229 -> 344,0 */
	reached7[229] = 1;
	trans[7][229]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][230]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[7][231]	= settr(1711,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][254]	= settr(1734,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][232]	= settr(1712,2,245,221,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][245] = settr(1725,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1725,0,236,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][236] = settr(1716,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1716,0,233,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][233]	= settr(1713,2,235,222,222,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][235] = settr(1715,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1715,0,234,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][234]	= settr(1714,2,240,223,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][240] = settr(1720,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1720,0,237,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][237]	= settr(1717,2,239,224,224,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][239] = settr(1719,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1719,0,238,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][238]	= settr(1718,2,244,225,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][244] = settr(1724,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1724,0,241,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][241]	= settr(1721,2,243,226,226,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][243] = settr(1723,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1723,0,242,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][242]	= settr(1722,2,247,227,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][247] = settr(1727,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1727,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][246]	= settr(1726,2,344,228,228,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 248 -> 344,0 */
	reached7[248] = 1;
	trans[7][248]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][249]	= settr(1729,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][250]	= settr(1730,2,251,2,0,"else", 1, 2500, 0);
	trans[7][251]	= settr(1731,2,344,229,229,"transition_id = -(1)", 1, 2500, 0);
	trans[7][252]	= settr(1732,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][255]	= settr(1735,2,257,230,230,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 7][257] = settr(1737,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1737,0,256,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][256]	= settr(1736,2,287,1,0,"(1)", 1, 2500, 0);
	T = trans[7][287] = settr(1767,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1767,2,258,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1767,2,266,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1767,2,284,0,0,"IF", 1, 2500, 0);
	trans[7][258]	= settr(1738,2,260,231,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][260] = settr(1740,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1740,0,259,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][259]	= settr(1739,2,262,232,232,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 7][262] = settr(1742,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1742,0,261,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][261]	= settr(1741,2,344,233,233,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 263 -> 344,0 */
	reached7[263] = 1;
	trans[7][263]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][264]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[7][265]	= settr(1745,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][288]	= settr(1768,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][266]	= settr(1746,2,279,234,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][279] = settr(1759,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1759,0,270,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][270] = settr(1750,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1750,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][267]	= settr(1747,2,269,235,235,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][269] = settr(1749,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1749,0,268,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][268]	= settr(1748,2,274,236,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][274] = settr(1754,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1754,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][271]	= settr(1751,2,273,237,237,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][273] = settr(1753,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1753,0,272,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][272]	= settr(1752,2,278,238,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][278] = settr(1758,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1758,0,275,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][275]	= settr(1755,2,277,239,239,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][277] = settr(1757,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1757,0,276,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][276]	= settr(1756,2,281,240,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][281] = settr(1761,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1761,0,280,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][280]	= settr(1760,2,344,241,241,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 282 -> 344,0 */
	reached7[282] = 1;
	trans[7][282]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][283]	= settr(1763,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][284]	= settr(1764,2,285,2,0,"else", 1, 2500, 0);
	trans[7][285]	= settr(1765,2,344,242,242,"transition_id = -(1)", 1, 2500, 0);
	trans[7][286]	= settr(1766,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][289]	= settr(1769,2,291,243,243,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 7][291] = settr(1771,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1771,0,290,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][290]	= settr(1770,2,298,1,0,"(1)", 1, 2500, 0);
	T = trans[7][298] = settr(1778,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1778,2,292,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1778,2,295,0,0,"IF", 1, 2500, 0);
	trans[7][292]	= settr(1772,2,294,244,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 7][294] = settr(1774,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1774,0,293,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][293]	= settr(1773,2,306,245,245,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[7][299]	= settr(1779,2,306,1,0,".(goto)", 1, 2500, 0);
	trans[7][295]	= settr(1775,2,297,2,0,"else", 1, 2500, 0);
	T = trans[ 7][297] = settr(1777,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1777,0,296,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][296]	= settr(1776,2,306,246,246,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[7][306] = settr(1786,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1786,2,300,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1786,2,303,0,0,"IF", 1, 2500, 0);
	trans[7][300]	= settr(1780,2,302,247,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 7][302] = settr(1782,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1782,0,301,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][301]	= settr(1781,2,344,248,248,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 308 -> 344,0 */
	reached7[308] = 1;
	trans[7][307]	= settr(1787,2,308,1,0,".(goto)", 1, 2500, 0); /* m: 308 -> 0,344 */
	reached7[308] = 1;
	trans[7][303]	= settr(1783,2,305,2,0,"else", 1, 2500, 0);
	T = trans[ 7][305] = settr(1785,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1785,0,304,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][304]	= settr(1784,2,344,249,249,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 308 -> 344,0 */
	reached7[308] = 1;
	trans[7][308]	= settr(1788,2,344,250,250,"transition_id = -(1)", 1, 2500, 0);
	trans[7][309]	= settr(1789,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][310]	= settr(1790,2,312,251,251,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 7][312] = settr(1792,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1792,0,311,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][311]	= settr(1791,2,324,1,0,"(1)", 1, 2500, 0);
	trans[7][313]	= settr(1793,2,324,1,0,"goto startcheck", 1, 2500, 0);
	trans[7][314]	= settr(1794,2,316,252,252,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 7][316] = settr(1796,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1796,0,315,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][315]	= settr(1795,2,324,1,0,"(1)", 1, 2500, 0);
	T = trans[7][324] = settr(1804,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1804,2,317,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1804,2,320,0,0,"IF", 1, 2500, 0);
	trans[7][317]	= settr(1797,2,344,253,253,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 318 -> 344,0 */
	reached7[318] = 1;
	trans[7][318]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][319]	= settr(1799,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][325]	= settr(1805,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][320]	= settr(1800,2,344,254,254,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 321 -> 344,0 */
	reached7[321] = 1;
	trans[7][321]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][322]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[7][323]	= settr(1803,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][326]	= settr(1806,2,328,255,255,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 7][328] = settr(1808,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1808,0,327,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][327]	= settr(1807,2,340,1,0,"(1)", 1, 2500, 0);
	trans[7][329]	= settr(1809,2,340,1,0,"goto stopcheck", 1, 2500, 0);
	trans[7][330]	= settr(1810,2,332,256,256,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 7][332] = settr(1812,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1812,0,331,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][331]	= settr(1811,2,340,1,0,"(1)", 1, 2500, 0);
	T = trans[7][340] = settr(1820,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1820,2,333,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1820,2,336,0,0,"IF", 1, 2500, 0);
	trans[7][333]	= settr(1813,2,344,257,257,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 334 -> 344,0 */
	reached7[334] = 1;
	trans[7][334]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][335]	= settr(1815,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][341]	= settr(1821,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][336]	= settr(1816,2,344,258,258,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 337 -> 344,0 */
	reached7[337] = 1;
	trans[7][337]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][338]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][339]	= settr(1819,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][344]	= settr(1824,2,345,1,0,"(1)", 1, 2500, 0);
	trans[7][347]	= settr(1827,2,354,1,0,"break", 1, 2500, 0);
	trans[7][349]	= settr(1829,2,350,2,0,"else", 1, 2500, 0);
	trans[7][350]	= settr(1830,2,354,1,0,"(1)", 1, 2500, 0);
	trans[7][354]	= settr(1834,2,465,259,259,"Untitled_msc_lock?_", 1, 2500, 0);
	T = trans[ 7][465] = settr(1945,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1945,0,463,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[7][463] = settr(1943,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1943,2,355,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1943,2,421,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1943,2,461,0,0,"IF", 1, 2500, 0);
	trans[7][355]	= settr(1835,2,381,260,0,"((global_state.untitled_msc.state==0))", 1, 2500, 0);
	T = trans[ 7][381] = settr(1861,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1861,0,356,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][356]	= settr(1836,2,379,261,261,"Untitled_MSC_0_sig0_param0.selection = Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0);
	T = trans[7][379] = settr(1859,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1859,2,357,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1859,2,362,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1859,2,372,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1859,2,377,0,0,"IF", 1, 2500, 0);
	trans[7][357]	= settr(1837,2,361,262,0,"((Untitled_MSC_0_sig0_param0.selection==1))", 1, 2500, 0);
	T = trans[ 7][361] = settr(1841,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1841,0,358,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][358]	= settr(1838,2,360,263,263,"Untitled_MSC_0_sig0_param0.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle", 1, 2500, 0);
	T = trans[ 7][360] = settr(1840,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1840,0,359,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][359]	= settr(1839,2,420,264,0,"assert(((Untitled_MSC_0_sig0_param0.data.goToIdle>=0)&&(Untitled_MSC_0_sig0_param0.data.goToIdle<=0)))", 1, 2500, 0);
	trans[7][380]	= settr(1860,2,420,1,0,".(goto)", 1, 2500, 0);
	trans[7][362]	= settr(1842,2,371,265,0,"((Untitled_MSC_0_sig0_param0.selection==2))", 1, 2500, 0);
	T = trans[ 7][371] = settr(1851,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1851,0,366,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][366] = settr(1846,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1846,0,363,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][363]	= settr(1843,2,365,266,266,"Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][365] = settr(1845,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1845,0,364,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][364]	= settr(1844,2,370,267,0,"assert(((Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1>=0)&&(Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][370] = settr(1850,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1850,0,367,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][367]	= settr(1847,2,369,268,268,"Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][369] = settr(1849,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1849,0,368,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][368]	= settr(1848,2,420,269,0,"assert(((Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2>=0)&&(Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2<=255)))", 1, 2500, 0);
	trans[7][372]	= settr(1852,2,376,270,0,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0);
	T = trans[ 7][376] = settr(1856,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1856,0,373,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][373]	= settr(1853,2,375,271,271,"Untitled_MSC_0_sig0_param0.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport", 1, 2500, 0);
	T = trans[ 7][375] = settr(1855,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1855,0,374,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][374]	= settr(1854,2,420,272,0,"assert(((Untitled_MSC_0_sig0_param0.data.doReport>=0)&&(Untitled_MSC_0_sig0_param0.data.doReport<=0)))", 1, 2500, 0);
	trans[7][377]	= settr(1857,2,378,2,0,"else", 1, 2500, 0);
	trans[7][378]	= settr(1858,2,420,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][420] = settr(1900,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1900,0,382,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][382]	= settr(1862,2,417,273,273,"transition_id = 0", 1, 2500, 0); /* m: 383 -> 0,417 */
	reached7[383] = 1;
	trans[7][383]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[7][418]	= settr(1898,2,417,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][417] = settr(1897,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1897,2,414,0,0,"DO", 1, 2500, 0);
	T = trans[7][414] = settr(1894,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1894,2,384,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1894,2,386,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1894,2,390,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1894,2,402,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1894,2,406,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1894,2,410,0,0,"IF", 1, 2500, 0);
	trans[7][384]	= settr(1864,2,466,274,274,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][385]	= settr(1865,2,466,1,0,"goto :b31", 1, 2500, 0);
	trans[7][415]	= settr(1895,2,416,1,0,".(goto)", 1, 2500, 0);
	trans[7][386]	= settr(1866,2,416,275,275,"((transition_id==0))", 1, 2500, 0); /* m: 387 -> 416,0 */
	reached7[387] = 1;
	trans[7][387]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][388]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][389]	= settr(1869,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][390]	= settr(1870,2,397,276,276,"((transition_id==1))", 1, 2500, 0);
	T = trans[7][397] = settr(1877,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1877,2,391,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1877,2,395,0,0,"IF", 1, 2500, 0);
	trans[7][391]	= settr(1871,2,416,277,277,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 392 -> 416,0 */
	reached7[392] = 1;
	trans[7][392]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][393]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[7][394]	= settr(1874,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][398]	= settr(1878,2,399,1,0,".(goto)", 1, 2500, 0); /* m: 399 -> 0,416 */
	reached7[399] = 1;
	trans[7][395]	= settr(1875,2,396,2,0,"else", 1, 2500, 0);
	trans[7][396]	= settr(1876,2,416,278,278,"(1)", 1, 2500, 0); /* m: 399 -> 416,0 */
	reached7[399] = 1;
	trans[7][399]	= settr(1879,2,416,279,279,"transition_id = -(1)", 1, 2500, 0); /* m: 400 -> 0,416 */
	reached7[400] = 1;
	trans[7][400]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][401]	= settr(1881,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][402]	= settr(1882,2,416,280,280,"((transition_id==2))", 1, 2500, 0); /* m: 403 -> 416,0 */
	reached7[403] = 1;
	trans[7][403]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][404]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][405]	= settr(1885,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][406]	= settr(1886,2,416,281,281,"((transition_id==3))", 1, 2500, 0); /* m: 407 -> 416,0 */
	reached7[407] = 1;
	trans[7][407]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][408]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[7][409]	= settr(1889,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][410]	= settr(1890,2,416,282,282,"((transition_id==4))", 1, 2500, 0); /* m: 411 -> 416,0 */
	reached7[411] = 1;
	trans[7][411]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][412]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[7][413]	= settr(1893,2,416,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][416]	= settr(1896,2,417,1,0,"(1)", 1, 2500, 0);
	trans[7][419]	= settr(1899,2,466,1,0,"break", 1, 2500, 0);
	trans[7][464]	= settr(1944,2,466,1,0,".(goto)", 1, 2500, 0);
	trans[7][421]	= settr(1901,2,460,283,0,"((global_state.untitled_msc.state==2))", 1, 2500, 0);
	T = trans[ 7][460] = settr(1940,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1940,0,422,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][422]	= settr(1902,2,457,284,284,"transition_id = 0", 1, 2500, 0); /* m: 423 -> 0,457 */
	reached7[423] = 1;
	trans[7][423]	= settr(0,0,0,0,0,"transition_id = 4",0,0,0);
	trans[7][458]	= settr(1938,2,457,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][457] = settr(1937,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1937,2,454,0,0,"DO", 1, 2500, 0);
	T = trans[7][454] = settr(1934,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1934,2,424,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1934,2,426,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1934,2,430,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1934,2,442,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1934,2,446,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1934,2,450,0,0,"IF", 1, 2500, 0);
	trans[7][424]	= settr(1904,2,466,285,285,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][425]	= settr(1905,2,466,1,0,"goto :b32", 1, 2500, 0);
	trans[7][455]	= settr(1935,2,456,1,0,".(goto)", 1, 2500, 0);
	trans[7][426]	= settr(1906,2,456,286,286,"((transition_id==0))", 1, 2500, 0); /* m: 427 -> 456,0 */
	reached7[427] = 1;
	trans[7][427]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][428]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][429]	= settr(1909,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][430]	= settr(1910,2,437,287,287,"((transition_id==1))", 1, 2500, 0);
	T = trans[7][437] = settr(1917,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1917,2,431,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1917,2,435,0,0,"IF", 1, 2500, 0);
	trans[7][431]	= settr(1911,2,456,288,288,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 432 -> 456,0 */
	reached7[432] = 1;
	trans[7][432]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][433]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[7][434]	= settr(1914,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][438]	= settr(1918,2,439,1,0,".(goto)", 1, 2500, 0); /* m: 439 -> 0,456 */
	reached7[439] = 1;
	trans[7][435]	= settr(1915,2,436,2,0,"else", 1, 2500, 0);
	trans[7][436]	= settr(1916,2,456,289,289,"(1)", 1, 2500, 0); /* m: 439 -> 456,0 */
	reached7[439] = 1;
	trans[7][439]	= settr(1919,2,456,290,290,"transition_id = -(1)", 1, 2500, 0); /* m: 440 -> 0,456 */
	reached7[440] = 1;
	trans[7][440]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][441]	= settr(1921,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][442]	= settr(1922,2,456,291,291,"((transition_id==2))", 1, 2500, 0); /* m: 443 -> 456,0 */
	reached7[443] = 1;
	trans[7][443]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][444]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[7][445]	= settr(1925,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][446]	= settr(1926,2,456,292,292,"((transition_id==3))", 1, 2500, 0); /* m: 447 -> 456,0 */
	reached7[447] = 1;
	trans[7][447]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][448]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[7][449]	= settr(1929,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][450]	= settr(1930,2,456,293,293,"((transition_id==4))", 1, 2500, 0); /* m: 451 -> 456,0 */
	reached7[451] = 1;
	trans[7][451]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][452]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[7][453]	= settr(1933,2,456,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][456]	= settr(1936,2,457,1,0,"(1)", 1, 2500, 0);
	trans[7][459]	= settr(1939,2,466,1,0,"break", 1, 2500, 0);
	trans[7][461]	= settr(1941,2,462,2,0,"else", 1, 2500, 0);
	trans[7][462]	= settr(1942,2,466,1,0,"(1)", 1, 2500, 0);
	trans[7][466]	= settr(1946,2,470,294,294,"Untitled_msc_lock!1", 1, 2500, 0);
	trans[7][467]	= settr(1947,2,470,1,0,"goto Modemanager_systemtc_loop", 1, 2500, 0);
	trans[7][471]	= settr(1951,2,472,1,0,".(goto)", 1, 2500, 0);
	trans[7][468]	= settr(1948,2,469,295,0,"(empty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][469]	= settr(1949,2,472,296,296,"(1)", 1, 2500, 0); /* m: 471 -> 472,0 */
	reached7[471] = 1;
	trans[7][472]	= settr(1952,0,474,297,297,"Modemanager_lock!1", 1, 2500, 0);
	trans[7][476]	= settr(1956,0,477,1,0,"break", 0, 2, 0);
	trans[7][477]	= settr(1957,0,0,298,298,"-end-", 0, 3500, 0);

	/* proctype 6: Modemanager_reading */

	trans[6] = (Trans **) emalloc(172*sizeof(Trans *));

	trans[6][1]	= settr(1310,0,168,299,0,"(inited)", 1, 2, 0);
	trans[6][169]	= settr(1478,0,168,1,0,".(goto)", 0, 2, 0);
	T = trans[6][168] = settr(1477,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1477,0,167,0,0,"DO", 0, 2, 0);
	T = trans[ 6][167] = settr(1476,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1476,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[6][2]	= settr(1311,2,3,300,0,"(nempty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][3]	= settr(1312,2,164,301,301,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[6][164] = settr(1473,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1473,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1473,2,162,0,0,"IF", 1, 2500, 0);
	trans[6][4]	= settr(1313,2,5,302,0,"(nempty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][5]	= settr(1314,2,160,303,303,"Modemanager_reading_channel?Modemanager_reading_signal_parameter", 1, 2500, 0); /* m: 6 -> 160,0 */
	reached6[6] = 1;
	trans[6][6]	= settr(0,0,0,0,0,"Modemanager_reading_channel_used = 1",0,0,0);
	T = trans[ 6][160] = settr(1469,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1469,0,158,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[6][158] = settr(1467,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1467,2,7,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1467,2,156,0,0,"IF", 1, 2500, 0);
	trans[6][7]	= settr(1316,2,11,304,0,"((global_state.modemanager.state==1))", 1, 2500, 0);
	T = trans[ 6][11] = settr(1320,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1320,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][8]	= settr(1317,2,10,305,305,"global_state.modemanager.r = Modemanager_reading_signal_parameter", 1, 2500, 0);
	T = trans[ 6][10] = settr(1319,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1319,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][9]	= settr(1318,2,155,306,0,"assert(((global_state.modemanager.r>=0)&&(global_state.modemanager.r<=255)))", 1, 2500, 0);
	T = trans[ 6][155] = settr(1464,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1464,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][12]	= settr(1321,2,152,307,307,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached6[13] = 1;
	trans[6][13]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[6][153]	= settr(1462,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[6][152] = settr(1461,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1461,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[6][149] = settr(1458,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1458,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1458,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1458,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1458,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1458,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1458,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1458,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1458,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1458,2,137,0,0,"IF", 1, 2500, 0);
	trans[6][14]	= settr(1323,2,164,308,308,"((transition_id==-(1)))", 1, 2500, 0);
	trans[6][15]	= settr(1324,2,164,1,0,"goto :b27", 1, 2500, 0);
	trans[6][150]	= settr(1459,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][16]	= settr(1325,2,151,309,309,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached6[17] = 1;
	trans[6][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[6][19]	= settr(1328,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][20]	= settr(1329,2,22,310,310,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 6][22] = settr(1331,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1331,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][21]	= settr(1330,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[6][60] = settr(1369,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1369,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1369,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1369,2,57,0,0,"IF", 1, 2500, 0);
	trans[6][23]	= settr(1332,2,27,311,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 6][27] = settr(1336,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1336,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][24]	= settr(1333,2,26,312,312,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 6][26] = settr(1335,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1335,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][25]	= settr(1334,2,31,313,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 6][31] = settr(1340,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1340,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][28]	= settr(1337,2,30,314,314,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 6][30] = settr(1339,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1339,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][29]	= settr(1338,2,33,315,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 6][33] = settr(1342,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1342,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][32]	= settr(1341,2,35,316,316,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 6][35] = settr(1344,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1344,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][34]	= settr(1343,2,151,317,317,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached6[36] = 1;
	trans[6][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[6][38]	= settr(1347,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][61]	= settr(1370,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][39]	= settr(1348,2,52,318,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 6][52] = settr(1361,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1361,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 6][43] = settr(1352,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1352,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][40]	= settr(1349,2,42,319,319,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 6][42] = settr(1351,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1351,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][41]	= settr(1350,2,47,320,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 6][47] = settr(1356,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1356,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][44]	= settr(1353,2,46,321,321,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 6][46] = settr(1355,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1355,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][45]	= settr(1354,2,51,322,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 6][51] = settr(1360,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1360,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][48]	= settr(1357,2,50,323,323,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 6][50] = settr(1359,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1359,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][49]	= settr(1358,2,54,324,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 6][54] = settr(1363,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1363,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][53]	= settr(1362,2,151,325,325,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached6[55] = 1;
	trans[6][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][56]	= settr(1365,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][57]	= settr(1366,2,58,2,0,"else", 1, 2500, 0);
	trans[6][58]	= settr(1367,2,151,326,326,"transition_id = -(1)", 1, 2500, 0);
	trans[6][59]	= settr(1368,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][62]	= settr(1371,2,64,327,327,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 6][64] = settr(1373,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1373,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][63]	= settr(1372,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[6][94] = settr(1403,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1403,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1403,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1403,2,91,0,0,"IF", 1, 2500, 0);
	trans[6][65]	= settr(1374,2,67,328,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 6][67] = settr(1376,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1376,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][66]	= settr(1375,2,69,329,329,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 6][69] = settr(1378,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1378,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][68]	= settr(1377,2,151,330,330,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached6[70] = 1;
	trans[6][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[6][72]	= settr(1381,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][95]	= settr(1404,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][73]	= settr(1382,2,86,331,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 6][86] = settr(1395,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1395,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 6][77] = settr(1386,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1386,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][74]	= settr(1383,2,76,332,332,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 6][76] = settr(1385,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1385,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][75]	= settr(1384,2,81,333,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 6][81] = settr(1390,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1390,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][78]	= settr(1387,2,80,334,334,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 6][80] = settr(1389,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1389,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][79]	= settr(1388,2,85,335,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 6][85] = settr(1394,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1394,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][82]	= settr(1391,2,84,336,336,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 6][84] = settr(1393,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1393,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][83]	= settr(1392,2,88,337,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 6][88] = settr(1397,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1397,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][87]	= settr(1396,2,151,338,338,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached6[89] = 1;
	trans[6][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][90]	= settr(1399,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][91]	= settr(1400,2,92,2,0,"else", 1, 2500, 0);
	trans[6][92]	= settr(1401,2,151,339,339,"transition_id = -(1)", 1, 2500, 0);
	trans[6][93]	= settr(1402,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][96]	= settr(1405,2,98,340,340,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 6][98] = settr(1407,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1407,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][97]	= settr(1406,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[6][105] = settr(1414,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1414,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1414,2,102,0,0,"IF", 1, 2500, 0);
	trans[6][99]	= settr(1408,2,101,341,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 6][101] = settr(1410,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1410,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][100]	= settr(1409,2,113,342,342,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[6][106]	= settr(1415,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[6][102]	= settr(1411,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 6][104] = settr(1413,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1413,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][103]	= settr(1412,2,113,343,343,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[6][113] = settr(1422,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1422,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1422,2,110,0,0,"IF", 1, 2500, 0);
	trans[6][107]	= settr(1416,2,109,344,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 6][109] = settr(1418,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1418,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][108]	= settr(1417,2,151,345,345,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached6[115] = 1;
	trans[6][114]	= settr(1423,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached6[115] = 1;
	trans[6][110]	= settr(1419,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 6][112] = settr(1421,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1421,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][111]	= settr(1420,2,151,346,346,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached6[115] = 1;
	trans[6][115]	= settr(1424,2,151,347,347,"transition_id = -(1)", 1, 2500, 0);
	trans[6][116]	= settr(1425,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][117]	= settr(1426,2,119,348,348,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 6][119] = settr(1428,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1428,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][118]	= settr(1427,2,131,1,0,"(1)", 1, 2500, 0);
	trans[6][120]	= settr(1429,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[6][121]	= settr(1430,2,123,349,349,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 6][123] = settr(1432,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1432,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][122]	= settr(1431,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[6][131] = settr(1440,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1440,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1440,2,127,0,0,"IF", 1, 2500, 0);
	trans[6][124]	= settr(1433,2,151,350,350,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached6[125] = 1;
	trans[6][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][126]	= settr(1435,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][132]	= settr(1441,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][127]	= settr(1436,2,151,351,351,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached6[128] = 1;
	trans[6][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[6][130]	= settr(1439,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][133]	= settr(1442,2,135,352,352,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 6][135] = settr(1444,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1444,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][134]	= settr(1443,2,147,1,0,"(1)", 1, 2500, 0);
	trans[6][136]	= settr(1445,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[6][137]	= settr(1446,2,139,353,353,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 6][139] = settr(1448,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1448,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][138]	= settr(1447,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[6][147] = settr(1456,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1456,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1456,2,143,0,0,"IF", 1, 2500, 0);
	trans[6][140]	= settr(1449,2,151,354,354,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached6[141] = 1;
	trans[6][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][142]	= settr(1451,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][148]	= settr(1457,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][143]	= settr(1452,2,151,355,355,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached6[144] = 1;
	trans[6][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[6][146]	= settr(1455,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][151]	= settr(1460,2,152,1,0,"(1)", 1, 2500, 0);
	trans[6][154]	= settr(1463,2,164,1,0,"break", 1, 2500, 0);
	trans[6][159]	= settr(1468,2,164,1,0,".(goto)", 1, 2500, 0);
	trans[6][156]	= settr(1465,2,157,2,0,"else", 1, 2500, 0);
	trans[6][157]	= settr(1466,2,164,1,0,"(1)", 1, 2500, 0);
	trans[6][161]	= settr(1470,2,164,1,0,"goto Modemanager_reading_loop", 1, 2500, 0);
	trans[6][165]	= settr(1474,2,166,1,0,".(goto)", 1, 2500, 0);
	trans[6][162]	= settr(1471,2,163,356,0,"(empty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][163]	= settr(1472,2,166,357,357,"(1)", 1, 2500, 0); /* m: 165 -> 166,0 */
	reached6[165] = 1;
	trans[6][166]	= settr(1475,0,168,358,358,"Modemanager_lock!1", 1, 2500, 0);
	trans[6][170]	= settr(1479,0,171,1,0,"break", 0, 2, 0);
	trans[6][171]	= settr(1480,0,0,359,359,"-end-", 0, 3500, 0);

	/* proctype 5: Modemanager_actuatorTm2 */

	trans[5] = (Trans **) emalloc(321*sizeof(Trans *));

	trans[5][1]	= settr(990,0,317,360,0,"(inited)", 1, 2, 0);
	trans[5][318]	= settr(1307,0,317,1,0,".(goto)", 0, 2, 0);
	T = trans[5][317] = settr(1306,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1306,0,316,0,0,"DO", 0, 2, 0);
	T = trans[ 5][316] = settr(1305,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1305,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[5][2]	= settr(991,2,3,361,0,"(nempty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][3]	= settr(992,2,313,362,362,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[5][313] = settr(1302,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1302,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1302,2,311,0,0,"IF", 1, 2500, 0);
	trans[5][4]	= settr(993,2,5,363,0,"(nempty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][5]	= settr(994,2,309,364,364,"Modemanager_actuatorTm2_channel?Modemanager_actuatortm2_signal_parameter", 1, 2500, 0); /* m: 6 -> 309,0 */
	reached5[6] = 1;
	trans[5][6]	= settr(0,0,0,0,0,"Modemanager_actuatortm2_channel_used = 1",0,0,0);
	T = trans[ 5][309] = settr(1298,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1298,0,307,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[5][307] = settr(1296,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1296,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1296,2,156,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1296,2,305,0,0,"IF", 1, 2500, 0);
	trans[5][7]	= settr(996,2,11,365,0,"((global_state.modemanager.state==0))", 1, 2500, 0);
	T = trans[ 5][11] = settr(1000,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1000,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][8]	= settr(997,2,10,366,366,"global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter", 1, 2500, 0);
	T = trans[ 5][10] = settr(999,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(999,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][9]	= settr(998,2,155,367,0,"assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))", 1, 2500, 0);
	T = trans[ 5][155] = settr(1144,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1144,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][12]	= settr(1001,2,152,368,368,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached5[13] = 1;
	trans[5][13]	= settr(0,0,0,0,0,"transition_id = 4",0,0,0);
	trans[5][153]	= settr(1142,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[5][152] = settr(1141,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1141,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[5][149] = settr(1138,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1138,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1138,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1138,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1138,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1138,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1138,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1138,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1138,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1138,2,137,0,0,"IF", 1, 2500, 0);
	trans[5][14]	= settr(1003,2,313,369,369,"((transition_id==-(1)))", 1, 2500, 0);
	trans[5][15]	= settr(1004,2,313,1,0,"goto :b24", 1, 2500, 0);
	trans[5][150]	= settr(1139,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][16]	= settr(1005,2,151,370,370,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached5[17] = 1;
	trans[5][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][19]	= settr(1008,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][20]	= settr(1009,2,22,371,371,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 5][22] = settr(1011,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1011,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][21]	= settr(1010,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[5][60] = settr(1049,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1049,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1049,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1049,2,57,0,0,"IF", 1, 2500, 0);
	trans[5][23]	= settr(1012,2,27,372,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 5][27] = settr(1016,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1016,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][24]	= settr(1013,2,26,373,373,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 5][26] = settr(1015,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1015,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][25]	= settr(1014,2,31,374,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 5][31] = settr(1020,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1020,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][28]	= settr(1017,2,30,375,375,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 5][30] = settr(1019,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1019,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][29]	= settr(1018,2,33,376,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 5][33] = settr(1022,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1022,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][32]	= settr(1021,2,35,377,377,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 5][35] = settr(1024,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1024,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][34]	= settr(1023,2,151,378,378,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached5[36] = 1;
	trans[5][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[5][38]	= settr(1027,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][61]	= settr(1050,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][39]	= settr(1028,2,52,379,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][52] = settr(1041,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1041,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][43] = settr(1032,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1032,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][40]	= settr(1029,2,42,380,380,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][42] = settr(1031,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1031,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][41]	= settr(1030,2,47,381,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][47] = settr(1036,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1036,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][44]	= settr(1033,2,46,382,382,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][46] = settr(1035,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1035,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][45]	= settr(1034,2,51,383,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][51] = settr(1040,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1040,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][48]	= settr(1037,2,50,384,384,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][50] = settr(1039,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1039,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][49]	= settr(1038,2,54,385,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][54] = settr(1043,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1043,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][53]	= settr(1042,2,151,386,386,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached5[55] = 1;
	trans[5][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][56]	= settr(1045,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][57]	= settr(1046,2,58,2,0,"else", 1, 2500, 0);
	trans[5][58]	= settr(1047,2,151,387,387,"transition_id = -(1)", 1, 2500, 0);
	trans[5][59]	= settr(1048,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][62]	= settr(1051,2,64,388,388,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 5][64] = settr(1053,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1053,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][63]	= settr(1052,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[5][94] = settr(1083,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1083,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1083,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1083,2,91,0,0,"IF", 1, 2500, 0);
	trans[5][65]	= settr(1054,2,67,389,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 5][67] = settr(1056,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1056,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][66]	= settr(1055,2,69,390,390,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 5][69] = settr(1058,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1058,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][68]	= settr(1057,2,151,391,391,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached5[70] = 1;
	trans[5][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[5][72]	= settr(1061,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][95]	= settr(1084,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][73]	= settr(1062,2,86,392,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][86] = settr(1075,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1075,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][77] = settr(1066,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1066,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][74]	= settr(1063,2,76,393,393,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][76] = settr(1065,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1065,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][75]	= settr(1064,2,81,394,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][81] = settr(1070,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1070,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][78]	= settr(1067,2,80,395,395,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][80] = settr(1069,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1069,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][79]	= settr(1068,2,85,396,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][85] = settr(1074,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1074,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][82]	= settr(1071,2,84,397,397,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][84] = settr(1073,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1073,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][83]	= settr(1072,2,88,398,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][88] = settr(1077,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1077,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][87]	= settr(1076,2,151,399,399,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached5[89] = 1;
	trans[5][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][90]	= settr(1079,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][91]	= settr(1080,2,92,2,0,"else", 1, 2500, 0);
	trans[5][92]	= settr(1081,2,151,400,400,"transition_id = -(1)", 1, 2500, 0);
	trans[5][93]	= settr(1082,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][96]	= settr(1085,2,98,401,401,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 5][98] = settr(1087,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1087,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][97]	= settr(1086,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[5][105] = settr(1094,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1094,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1094,2,102,0,0,"IF", 1, 2500, 0);
	trans[5][99]	= settr(1088,2,101,402,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 5][101] = settr(1090,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1090,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][100]	= settr(1089,2,113,403,403,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[5][106]	= settr(1095,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[5][102]	= settr(1091,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 5][104] = settr(1093,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1093,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][103]	= settr(1092,2,113,404,404,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[5][113] = settr(1102,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1102,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1102,2,110,0,0,"IF", 1, 2500, 0);
	trans[5][107]	= settr(1096,2,109,405,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 5][109] = settr(1098,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1098,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][108]	= settr(1097,2,151,406,406,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached5[115] = 1;
	trans[5][114]	= settr(1103,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached5[115] = 1;
	trans[5][110]	= settr(1099,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 5][112] = settr(1101,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1101,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][111]	= settr(1100,2,151,407,407,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached5[115] = 1;
	trans[5][115]	= settr(1104,2,151,408,408,"transition_id = -(1)", 1, 2500, 0);
	trans[5][116]	= settr(1105,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][117]	= settr(1106,2,119,409,409,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 5][119] = settr(1108,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1108,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][118]	= settr(1107,2,131,1,0,"(1)", 1, 2500, 0);
	trans[5][120]	= settr(1109,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[5][121]	= settr(1110,2,123,410,410,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 5][123] = settr(1112,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1112,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][122]	= settr(1111,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[5][131] = settr(1120,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1120,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1120,2,127,0,0,"IF", 1, 2500, 0);
	trans[5][124]	= settr(1113,2,151,411,411,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached5[125] = 1;
	trans[5][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][126]	= settr(1115,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][132]	= settr(1121,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][127]	= settr(1116,2,151,412,412,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached5[128] = 1;
	trans[5][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[5][130]	= settr(1119,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][133]	= settr(1122,2,135,413,413,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 5][135] = settr(1124,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1124,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][134]	= settr(1123,2,147,1,0,"(1)", 1, 2500, 0);
	trans[5][136]	= settr(1125,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[5][137]	= settr(1126,2,139,414,414,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 5][139] = settr(1128,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1128,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][138]	= settr(1127,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[5][147] = settr(1136,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1136,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1136,2,143,0,0,"IF", 1, 2500, 0);
	trans[5][140]	= settr(1129,2,151,415,415,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached5[141] = 1;
	trans[5][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][142]	= settr(1131,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][148]	= settr(1137,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][143]	= settr(1132,2,151,416,416,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached5[144] = 1;
	trans[5][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][146]	= settr(1135,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][151]	= settr(1140,2,152,1,0,"(1)", 1, 2500, 0);
	trans[5][154]	= settr(1143,2,313,1,0,"break", 1, 2500, 0);
	trans[5][308]	= settr(1297,2,313,1,0,".(goto)", 1, 2500, 0);
	trans[5][156]	= settr(1145,2,160,417,0,"((global_state.modemanager.state==2))", 1, 2500, 0);
	T = trans[ 5][160] = settr(1149,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1149,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][157]	= settr(1146,2,159,418,418,"global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter", 1, 2500, 0);
	T = trans[ 5][159] = settr(1148,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1148,0,158,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][158]	= settr(1147,2,304,419,0,"assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))", 1, 2500, 0);
	T = trans[ 5][304] = settr(1293,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1293,0,161,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][161]	= settr(1150,2,301,420,420,"transition_id = 0", 1, 2500, 0); /* m: 162 -> 0,301 */
	reached5[162] = 1;
	trans[5][162]	= settr(0,0,0,0,0,"transition_id = 6",0,0,0);
	trans[5][302]	= settr(1291,2,301,1,0,".(goto)", 1, 2500, 0);
	T = trans[5][301] = settr(1290,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1290,2,298,0,0,"DO", 1, 2500, 0);
	T = trans[5][298] = settr(1287,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1287,2,163,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1287,2,165,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1287,2,169,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1287,2,211,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1287,2,245,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1287,2,266,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1287,2,270,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1287,2,282,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1287,2,286,0,0,"IF", 1, 2500, 0);
	trans[5][163]	= settr(1152,2,313,421,421,"((transition_id==-(1)))", 1, 2500, 0);
	trans[5][164]	= settr(1153,2,313,1,0,"goto :b25", 1, 2500, 0);
	trans[5][299]	= settr(1288,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][165]	= settr(1154,2,300,422,422,"((transition_id==0))", 1, 2500, 0); /* m: 166 -> 300,0 */
	reached5[166] = 1;
	trans[5][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][168]	= settr(1157,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][169]	= settr(1158,2,171,423,423,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 5][171] = settr(1160,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1160,0,170,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][170]	= settr(1159,2,209,1,0,"(1)", 1, 2500, 0);
	T = trans[5][209] = settr(1198,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1198,2,172,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1198,2,188,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1198,2,206,0,0,"IF", 1, 2500, 0);
	trans[5][172]	= settr(1161,2,176,424,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 5][176] = settr(1165,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1165,0,173,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][173]	= settr(1162,2,175,425,425,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 5][175] = settr(1164,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1164,0,174,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][174]	= settr(1163,2,180,426,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 5][180] = settr(1169,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1169,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][177]	= settr(1166,2,179,427,427,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 5][179] = settr(1168,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1168,0,178,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][178]	= settr(1167,2,182,428,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 5][182] = settr(1171,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1171,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][181]	= settr(1170,2,184,429,429,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 5][184] = settr(1173,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1173,0,183,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][183]	= settr(1172,2,300,430,430,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 185 -> 300,0 */
	reached5[185] = 1;
	trans[5][185]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][186]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[5][187]	= settr(1176,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][210]	= settr(1199,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][188]	= settr(1177,2,201,431,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][201] = settr(1190,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1190,0,192,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][192] = settr(1181,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1181,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][189]	= settr(1178,2,191,432,432,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][191] = settr(1180,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1180,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][190]	= settr(1179,2,196,433,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][196] = settr(1185,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1185,0,193,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][193]	= settr(1182,2,195,434,434,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][195] = settr(1184,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1184,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][194]	= settr(1183,2,200,435,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][200] = settr(1189,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1189,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][197]	= settr(1186,2,199,436,436,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][199] = settr(1188,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1188,0,198,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][198]	= settr(1187,2,203,437,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][203] = settr(1192,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1192,0,202,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][202]	= settr(1191,2,300,438,438,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 204 -> 300,0 */
	reached5[204] = 1;
	trans[5][204]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][205]	= settr(1194,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][206]	= settr(1195,2,207,2,0,"else", 1, 2500, 0);
	trans[5][207]	= settr(1196,2,300,439,439,"transition_id = -(1)", 1, 2500, 0);
	trans[5][208]	= settr(1197,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][211]	= settr(1200,2,213,440,440,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 5][213] = settr(1202,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1202,0,212,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][212]	= settr(1201,2,243,1,0,"(1)", 1, 2500, 0);
	T = trans[5][243] = settr(1232,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1232,2,214,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1232,2,222,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1232,2,240,0,0,"IF", 1, 2500, 0);
	trans[5][214]	= settr(1203,2,216,441,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 5][216] = settr(1205,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1205,0,215,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][215]	= settr(1204,2,218,442,442,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 5][218] = settr(1207,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1207,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][217]	= settr(1206,2,300,443,443,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 219 -> 300,0 */
	reached5[219] = 1;
	trans[5][219]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][220]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[5][221]	= settr(1210,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][244]	= settr(1233,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][222]	= settr(1211,2,235,444,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][235] = settr(1224,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1224,0,226,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][226] = settr(1215,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1215,0,223,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][223]	= settr(1212,2,225,445,445,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][225] = settr(1214,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1214,0,224,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][224]	= settr(1213,2,230,446,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][230] = settr(1219,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1219,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][227]	= settr(1216,2,229,447,447,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][229] = settr(1218,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1218,0,228,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][228]	= settr(1217,2,234,448,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][234] = settr(1223,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1223,0,231,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][231]	= settr(1220,2,233,449,449,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][233] = settr(1222,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1222,0,232,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][232]	= settr(1221,2,237,450,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][237] = settr(1226,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1226,0,236,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][236]	= settr(1225,2,300,451,451,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 238 -> 300,0 */
	reached5[238] = 1;
	trans[5][238]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][239]	= settr(1228,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][240]	= settr(1229,2,241,2,0,"else", 1, 2500, 0);
	trans[5][241]	= settr(1230,2,300,452,452,"transition_id = -(1)", 1, 2500, 0);
	trans[5][242]	= settr(1231,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][245]	= settr(1234,2,247,453,453,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 5][247] = settr(1236,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1236,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][246]	= settr(1235,2,254,1,0,"(1)", 1, 2500, 0);
	T = trans[5][254] = settr(1243,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1243,2,248,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1243,2,251,0,0,"IF", 1, 2500, 0);
	trans[5][248]	= settr(1237,2,250,454,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 5][250] = settr(1239,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1239,0,249,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][249]	= settr(1238,2,262,455,455,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[5][255]	= settr(1244,2,262,1,0,".(goto)", 1, 2500, 0);
	trans[5][251]	= settr(1240,2,253,2,0,"else", 1, 2500, 0);
	T = trans[ 5][253] = settr(1242,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1242,0,252,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][252]	= settr(1241,2,262,456,456,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[5][262] = settr(1251,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1251,2,256,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1251,2,259,0,0,"IF", 1, 2500, 0);
	trans[5][256]	= settr(1245,2,258,457,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 5][258] = settr(1247,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1247,0,257,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][257]	= settr(1246,2,300,458,458,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached5[264] = 1;
	trans[5][263]	= settr(1252,2,264,1,0,".(goto)", 1, 2500, 0); /* m: 264 -> 0,300 */
	reached5[264] = 1;
	trans[5][259]	= settr(1248,2,261,2,0,"else", 1, 2500, 0);
	T = trans[ 5][261] = settr(1250,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1250,0,260,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][260]	= settr(1249,2,300,459,459,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached5[264] = 1;
	trans[5][264]	= settr(1253,2,300,460,460,"transition_id = -(1)", 1, 2500, 0);
	trans[5][265]	= settr(1254,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][266]	= settr(1255,2,268,461,461,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 5][268] = settr(1257,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1257,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][267]	= settr(1256,2,280,1,0,"(1)", 1, 2500, 0);
	trans[5][269]	= settr(1258,2,280,1,0,"goto startcheck", 1, 2500, 0);
	trans[5][270]	= settr(1259,2,272,462,462,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 5][272] = settr(1261,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1261,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][271]	= settr(1260,2,280,1,0,"(1)", 1, 2500, 0);
	T = trans[5][280] = settr(1269,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1269,2,273,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1269,2,276,0,0,"IF", 1, 2500, 0);
	trans[5][273]	= settr(1262,2,300,463,463,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 274 -> 300,0 */
	reached5[274] = 1;
	trans[5][274]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][275]	= settr(1264,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][281]	= settr(1270,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][276]	= settr(1265,2,300,464,464,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 277 -> 300,0 */
	reached5[277] = 1;
	trans[5][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][278]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[5][279]	= settr(1268,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][282]	= settr(1271,2,284,465,465,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 5][284] = settr(1273,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1273,0,283,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][283]	= settr(1272,2,296,1,0,"(1)", 1, 2500, 0);
	trans[5][285]	= settr(1274,2,296,1,0,"goto stopcheck", 1, 2500, 0);
	trans[5][286]	= settr(1275,2,288,466,466,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 5][288] = settr(1277,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1277,0,287,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][287]	= settr(1276,2,296,1,0,"(1)", 1, 2500, 0);
	T = trans[5][296] = settr(1285,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1285,2,289,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1285,2,292,0,0,"IF", 1, 2500, 0);
	trans[5][289]	= settr(1278,2,300,467,467,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 290 -> 300,0 */
	reached5[290] = 1;
	trans[5][290]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][291]	= settr(1280,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][297]	= settr(1286,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][292]	= settr(1281,2,300,468,468,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 293 -> 300,0 */
	reached5[293] = 1;
	trans[5][293]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][294]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][295]	= settr(1284,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][300]	= settr(1289,2,301,1,0,"(1)", 1, 2500, 0);
	trans[5][303]	= settr(1292,2,313,1,0,"break", 1, 2500, 0);
	trans[5][305]	= settr(1294,2,306,2,0,"else", 1, 2500, 0);
	trans[5][306]	= settr(1295,2,313,1,0,"(1)", 1, 2500, 0);
	trans[5][310]	= settr(1299,2,313,1,0,"goto Modemanager_actuatortm2_loop", 1, 2500, 0);
	trans[5][314]	= settr(1303,2,315,1,0,".(goto)", 1, 2500, 0);
	trans[5][311]	= settr(1300,2,312,469,0,"(empty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][312]	= settr(1301,2,315,470,470,"(1)", 1, 2500, 0); /* m: 314 -> 315,0 */
	reached5[314] = 1;
	trans[5][315]	= settr(1304,0,317,471,471,"Modemanager_lock!1", 1, 2500, 0);
	trans[5][319]	= settr(1308,0,320,1,0,"break", 0, 2, 0);
	trans[5][320]	= settr(1309,0,0,472,472,"-end-", 0, 3500, 0);

	/* proctype 4: Modemanager_actuatorTm1 */

	trans[4] = (Trans **) emalloc(321*sizeof(Trans *));

	trans[4][1]	= settr(670,0,317,473,0,"(inited)", 1, 2, 0);
	trans[4][318]	= settr(987,0,317,1,0,".(goto)", 0, 2, 0);
	T = trans[4][317] = settr(986,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(986,0,316,0,0,"DO", 0, 2, 0);
	T = trans[ 4][316] = settr(985,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(985,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[4][2]	= settr(671,2,3,474,0,"(nempty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][3]	= settr(672,2,313,475,475,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[4][313] = settr(982,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(982,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(982,2,311,0,0,"IF", 1, 2500, 0);
	trans[4][4]	= settr(673,2,5,476,0,"(nempty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][5]	= settr(674,2,309,477,477,"Modemanager_actuatorTm1_channel?Modemanager_actuatortm1_signal_parameter", 1, 2500, 0); /* m: 6 -> 309,0 */
	reached4[6] = 1;
	trans[4][6]	= settr(0,0,0,0,0,"Modemanager_actuatortm1_channel_used = 1",0,0,0);
	T = trans[ 4][309] = settr(978,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(978,0,307,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[4][307] = settr(976,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(976,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(976,2,156,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(976,2,305,0,0,"IF", 1, 2500, 0);
	trans[4][7]	= settr(676,2,11,478,0,"((global_state.modemanager.state==0))", 1, 2500, 0);
	T = trans[ 4][11] = settr(680,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(680,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][8]	= settr(677,2,10,479,479,"global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter", 1, 2500, 0);
	T = trans[ 4][10] = settr(679,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(679,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][9]	= settr(678,2,155,480,0,"assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))", 1, 2500, 0);
	T = trans[ 4][155] = settr(824,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(824,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][12]	= settr(681,2,152,481,481,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached4[13] = 1;
	trans[4][13]	= settr(0,0,0,0,0,"transition_id = 5",0,0,0);
	trans[4][153]	= settr(822,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[4][152] = settr(821,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(821,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[4][149] = settr(818,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(818,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(818,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(818,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(818,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(818,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(818,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(818,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(818,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(818,2,137,0,0,"IF", 1, 2500, 0);
	trans[4][14]	= settr(683,2,313,482,482,"((transition_id==-(1)))", 1, 2500, 0);
	trans[4][15]	= settr(684,2,313,1,0,"goto :b21", 1, 2500, 0);
	trans[4][150]	= settr(819,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][16]	= settr(685,2,151,483,483,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached4[17] = 1;
	trans[4][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][19]	= settr(688,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][20]	= settr(689,2,22,484,484,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 4][22] = settr(691,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(691,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][21]	= settr(690,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[4][60] = settr(729,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(729,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(729,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(729,2,57,0,0,"IF", 1, 2500, 0);
	trans[4][23]	= settr(692,2,27,485,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 4][27] = settr(696,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(696,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][24]	= settr(693,2,26,486,486,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 4][26] = settr(695,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(695,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][25]	= settr(694,2,31,487,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 4][31] = settr(700,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(700,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][28]	= settr(697,2,30,488,488,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 4][30] = settr(699,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(699,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][29]	= settr(698,2,33,489,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 4][33] = settr(702,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(702,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][32]	= settr(701,2,35,490,490,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 4][35] = settr(704,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(704,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][34]	= settr(703,2,151,491,491,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached4[36] = 1;
	trans[4][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[4][38]	= settr(707,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][61]	= settr(730,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][39]	= settr(708,2,52,492,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][52] = settr(721,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(721,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][43] = settr(712,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(712,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][40]	= settr(709,2,42,493,493,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][42] = settr(711,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(711,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][41]	= settr(710,2,47,494,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][47] = settr(716,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(716,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][44]	= settr(713,2,46,495,495,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][46] = settr(715,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(715,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][45]	= settr(714,2,51,496,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][51] = settr(720,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(720,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][48]	= settr(717,2,50,497,497,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][50] = settr(719,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(719,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][49]	= settr(718,2,54,498,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][54] = settr(723,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(723,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][53]	= settr(722,2,151,499,499,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached4[55] = 1;
	trans[4][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][56]	= settr(725,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][57]	= settr(726,2,58,2,0,"else", 1, 2500, 0);
	trans[4][58]	= settr(727,2,151,500,500,"transition_id = -(1)", 1, 2500, 0);
	trans[4][59]	= settr(728,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][62]	= settr(731,2,64,501,501,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 4][64] = settr(733,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(733,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][63]	= settr(732,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[4][94] = settr(763,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(763,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(763,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(763,2,91,0,0,"IF", 1, 2500, 0);
	trans[4][65]	= settr(734,2,67,502,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 4][67] = settr(736,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(736,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][66]	= settr(735,2,69,503,503,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 4][69] = settr(738,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(738,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][68]	= settr(737,2,151,504,504,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached4[70] = 1;
	trans[4][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[4][72]	= settr(741,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][95]	= settr(764,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][73]	= settr(742,2,86,505,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][86] = settr(755,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(755,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][77] = settr(746,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(746,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][74]	= settr(743,2,76,506,506,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][76] = settr(745,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(745,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][75]	= settr(744,2,81,507,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][81] = settr(750,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(750,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][78]	= settr(747,2,80,508,508,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][80] = settr(749,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(749,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][79]	= settr(748,2,85,509,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][85] = settr(754,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(754,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][82]	= settr(751,2,84,510,510,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][84] = settr(753,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(753,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][83]	= settr(752,2,88,511,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][88] = settr(757,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(757,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][87]	= settr(756,2,151,512,512,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached4[89] = 1;
	trans[4][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][90]	= settr(759,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][91]	= settr(760,2,92,2,0,"else", 1, 2500, 0);
	trans[4][92]	= settr(761,2,151,513,513,"transition_id = -(1)", 1, 2500, 0);
	trans[4][93]	= settr(762,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][96]	= settr(765,2,98,514,514,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 4][98] = settr(767,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(767,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][97]	= settr(766,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[4][105] = settr(774,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(774,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(774,2,102,0,0,"IF", 1, 2500, 0);
	trans[4][99]	= settr(768,2,101,515,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 4][101] = settr(770,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(770,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][100]	= settr(769,2,113,516,516,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[4][106]	= settr(775,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[4][102]	= settr(771,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 4][104] = settr(773,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(773,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][103]	= settr(772,2,113,517,517,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[4][113] = settr(782,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(782,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(782,2,110,0,0,"IF", 1, 2500, 0);
	trans[4][107]	= settr(776,2,109,518,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 4][109] = settr(778,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(778,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][108]	= settr(777,2,151,519,519,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached4[115] = 1;
	trans[4][114]	= settr(783,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached4[115] = 1;
	trans[4][110]	= settr(779,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 4][112] = settr(781,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(781,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][111]	= settr(780,2,151,520,520,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached4[115] = 1;
	trans[4][115]	= settr(784,2,151,521,521,"transition_id = -(1)", 1, 2500, 0);
	trans[4][116]	= settr(785,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][117]	= settr(786,2,119,522,522,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 4][119] = settr(788,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(788,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][118]	= settr(787,2,131,1,0,"(1)", 1, 2500, 0);
	trans[4][120]	= settr(789,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[4][121]	= settr(790,2,123,523,523,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 4][123] = settr(792,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(792,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][122]	= settr(791,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[4][131] = settr(800,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(800,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(800,2,127,0,0,"IF", 1, 2500, 0);
	trans[4][124]	= settr(793,2,151,524,524,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached4[125] = 1;
	trans[4][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][126]	= settr(795,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][132]	= settr(801,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][127]	= settr(796,2,151,525,525,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached4[128] = 1;
	trans[4][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[4][130]	= settr(799,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][133]	= settr(802,2,135,526,526,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 4][135] = settr(804,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(804,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][134]	= settr(803,2,147,1,0,"(1)", 1, 2500, 0);
	trans[4][136]	= settr(805,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[4][137]	= settr(806,2,139,527,527,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 4][139] = settr(808,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(808,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][138]	= settr(807,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[4][147] = settr(816,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(816,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(816,2,143,0,0,"IF", 1, 2500, 0);
	trans[4][140]	= settr(809,2,151,528,528,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached4[141] = 1;
	trans[4][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][142]	= settr(811,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][148]	= settr(817,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][143]	= settr(812,2,151,529,529,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached4[144] = 1;
	trans[4][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][146]	= settr(815,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][151]	= settr(820,2,152,1,0,"(1)", 1, 2500, 0);
	trans[4][154]	= settr(823,2,313,1,0,"break", 1, 2500, 0);
	trans[4][308]	= settr(977,2,313,1,0,".(goto)", 1, 2500, 0);
	trans[4][156]	= settr(825,2,160,530,0,"((global_state.modemanager.state==2))", 1, 2500, 0);
	T = trans[ 4][160] = settr(829,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(829,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][157]	= settr(826,2,159,531,531,"global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter", 1, 2500, 0);
	T = trans[ 4][159] = settr(828,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(828,0,158,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][158]	= settr(827,2,304,532,0,"assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))", 1, 2500, 0);
	T = trans[ 4][304] = settr(973,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(973,0,161,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][161]	= settr(830,2,301,533,533,"transition_id = 0", 1, 2500, 0); /* m: 162 -> 0,301 */
	reached4[162] = 1;
	trans[4][162]	= settr(0,0,0,0,0,"transition_id = 7",0,0,0);
	trans[4][302]	= settr(971,2,301,1,0,".(goto)", 1, 2500, 0);
	T = trans[4][301] = settr(970,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(970,2,298,0,0,"DO", 1, 2500, 0);
	T = trans[4][298] = settr(967,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(967,2,163,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(967,2,165,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(967,2,169,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(967,2,211,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(967,2,245,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(967,2,266,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(967,2,270,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(967,2,282,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(967,2,286,0,0,"IF", 1, 2500, 0);
	trans[4][163]	= settr(832,2,313,534,534,"((transition_id==-(1)))", 1, 2500, 0);
	trans[4][164]	= settr(833,2,313,1,0,"goto :b22", 1, 2500, 0);
	trans[4][299]	= settr(968,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][165]	= settr(834,2,300,535,535,"((transition_id==0))", 1, 2500, 0); /* m: 166 -> 300,0 */
	reached4[166] = 1;
	trans[4][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][168]	= settr(837,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][169]	= settr(838,2,171,536,536,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 4][171] = settr(840,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(840,0,170,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][170]	= settr(839,2,209,1,0,"(1)", 1, 2500, 0);
	T = trans[4][209] = settr(878,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(878,2,172,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(878,2,188,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(878,2,206,0,0,"IF", 1, 2500, 0);
	trans[4][172]	= settr(841,2,176,537,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 4][176] = settr(845,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(845,0,173,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][173]	= settr(842,2,175,538,538,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 4][175] = settr(844,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(844,0,174,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][174]	= settr(843,2,180,539,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 4][180] = settr(849,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(849,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][177]	= settr(846,2,179,540,540,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 4][179] = settr(848,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(848,0,178,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][178]	= settr(847,2,182,541,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 4][182] = settr(851,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(851,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][181]	= settr(850,2,184,542,542,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 4][184] = settr(853,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(853,0,183,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][183]	= settr(852,2,300,543,543,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 185 -> 300,0 */
	reached4[185] = 1;
	trans[4][185]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][186]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[4][187]	= settr(856,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][210]	= settr(879,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][188]	= settr(857,2,201,544,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][201] = settr(870,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(870,0,192,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][192] = settr(861,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(861,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][189]	= settr(858,2,191,545,545,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][191] = settr(860,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(860,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][190]	= settr(859,2,196,546,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][196] = settr(865,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(865,0,193,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][193]	= settr(862,2,195,547,547,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][195] = settr(864,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(864,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][194]	= settr(863,2,200,548,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][200] = settr(869,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(869,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][197]	= settr(866,2,199,549,549,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][199] = settr(868,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(868,0,198,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][198]	= settr(867,2,203,550,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][203] = settr(872,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(872,0,202,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][202]	= settr(871,2,300,551,551,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 204 -> 300,0 */
	reached4[204] = 1;
	trans[4][204]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][205]	= settr(874,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][206]	= settr(875,2,207,2,0,"else", 1, 2500, 0);
	trans[4][207]	= settr(876,2,300,552,552,"transition_id = -(1)", 1, 2500, 0);
	trans[4][208]	= settr(877,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][211]	= settr(880,2,213,553,553,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 4][213] = settr(882,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(882,0,212,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][212]	= settr(881,2,243,1,0,"(1)", 1, 2500, 0);
	T = trans[4][243] = settr(912,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(912,2,214,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(912,2,222,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(912,2,240,0,0,"IF", 1, 2500, 0);
	trans[4][214]	= settr(883,2,216,554,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 4][216] = settr(885,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(885,0,215,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][215]	= settr(884,2,218,555,555,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 4][218] = settr(887,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(887,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][217]	= settr(886,2,300,556,556,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 219 -> 300,0 */
	reached4[219] = 1;
	trans[4][219]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][220]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[4][221]	= settr(890,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][244]	= settr(913,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][222]	= settr(891,2,235,557,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][235] = settr(904,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(904,0,226,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][226] = settr(895,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(895,0,223,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][223]	= settr(892,2,225,558,558,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][225] = settr(894,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(894,0,224,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][224]	= settr(893,2,230,559,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][230] = settr(899,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(899,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][227]	= settr(896,2,229,560,560,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][229] = settr(898,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(898,0,228,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][228]	= settr(897,2,234,561,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][234] = settr(903,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(903,0,231,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][231]	= settr(900,2,233,562,562,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][233] = settr(902,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(902,0,232,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][232]	= settr(901,2,237,563,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][237] = settr(906,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(906,0,236,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][236]	= settr(905,2,300,564,564,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 238 -> 300,0 */
	reached4[238] = 1;
	trans[4][238]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][239]	= settr(908,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][240]	= settr(909,2,241,2,0,"else", 1, 2500, 0);
	trans[4][241]	= settr(910,2,300,565,565,"transition_id = -(1)", 1, 2500, 0);
	trans[4][242]	= settr(911,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][245]	= settr(914,2,247,566,566,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 4][247] = settr(916,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(916,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][246]	= settr(915,2,254,1,0,"(1)", 1, 2500, 0);
	T = trans[4][254] = settr(923,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(923,2,248,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(923,2,251,0,0,"IF", 1, 2500, 0);
	trans[4][248]	= settr(917,2,250,567,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 4][250] = settr(919,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(919,0,249,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][249]	= settr(918,2,262,568,568,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[4][255]	= settr(924,2,262,1,0,".(goto)", 1, 2500, 0);
	trans[4][251]	= settr(920,2,253,2,0,"else", 1, 2500, 0);
	T = trans[ 4][253] = settr(922,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(922,0,252,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][252]	= settr(921,2,262,569,569,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[4][262] = settr(931,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(931,2,256,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(931,2,259,0,0,"IF", 1, 2500, 0);
	trans[4][256]	= settr(925,2,258,570,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 4][258] = settr(927,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(927,0,257,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][257]	= settr(926,2,300,571,571,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached4[264] = 1;
	trans[4][263]	= settr(932,2,264,1,0,".(goto)", 1, 2500, 0); /* m: 264 -> 0,300 */
	reached4[264] = 1;
	trans[4][259]	= settr(928,2,261,2,0,"else", 1, 2500, 0);
	T = trans[ 4][261] = settr(930,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(930,0,260,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][260]	= settr(929,2,300,572,572,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached4[264] = 1;
	trans[4][264]	= settr(933,2,300,573,573,"transition_id = -(1)", 1, 2500, 0);
	trans[4][265]	= settr(934,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][266]	= settr(935,2,268,574,574,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 4][268] = settr(937,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(937,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][267]	= settr(936,2,280,1,0,"(1)", 1, 2500, 0);
	trans[4][269]	= settr(938,2,280,1,0,"goto startcheck", 1, 2500, 0);
	trans[4][270]	= settr(939,2,272,575,575,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 4][272] = settr(941,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(941,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][271]	= settr(940,2,280,1,0,"(1)", 1, 2500, 0);
	T = trans[4][280] = settr(949,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(949,2,273,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(949,2,276,0,0,"IF", 1, 2500, 0);
	trans[4][273]	= settr(942,2,300,576,576,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 274 -> 300,0 */
	reached4[274] = 1;
	trans[4][274]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][275]	= settr(944,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][281]	= settr(950,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][276]	= settr(945,2,300,577,577,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 277 -> 300,0 */
	reached4[277] = 1;
	trans[4][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][278]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[4][279]	= settr(948,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][282]	= settr(951,2,284,578,578,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 4][284] = settr(953,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(953,0,283,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][283]	= settr(952,2,296,1,0,"(1)", 1, 2500, 0);
	trans[4][285]	= settr(954,2,296,1,0,"goto stopcheck", 1, 2500, 0);
	trans[4][286]	= settr(955,2,288,579,579,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 4][288] = settr(957,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(957,0,287,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][287]	= settr(956,2,296,1,0,"(1)", 1, 2500, 0);
	T = trans[4][296] = settr(965,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(965,2,289,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(965,2,292,0,0,"IF", 1, 2500, 0);
	trans[4][289]	= settr(958,2,300,580,580,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 290 -> 300,0 */
	reached4[290] = 1;
	trans[4][290]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][291]	= settr(960,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][297]	= settr(966,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][292]	= settr(961,2,300,581,581,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 293 -> 300,0 */
	reached4[293] = 1;
	trans[4][293]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][294]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][295]	= settr(964,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][300]	= settr(969,2,301,1,0,"(1)", 1, 2500, 0);
	trans[4][303]	= settr(972,2,313,1,0,"break", 1, 2500, 0);
	trans[4][305]	= settr(974,2,306,2,0,"else", 1, 2500, 0);
	trans[4][306]	= settr(975,2,313,1,0,"(1)", 1, 2500, 0);
	trans[4][310]	= settr(979,2,313,1,0,"goto Modemanager_actuatortm1_loop", 1, 2500, 0);
	trans[4][314]	= settr(983,2,315,1,0,".(goto)", 1, 2500, 0);
	trans[4][311]	= settr(980,2,312,582,0,"(empty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][312]	= settr(981,2,315,583,583,"(1)", 1, 2500, 0); /* m: 314 -> 315,0 */
	reached4[314] = 1;
	trans[4][315]	= settr(984,0,317,584,584,"Modemanager_lock!1", 1, 2500, 0);
	trans[4][319]	= settr(988,0,320,1,0,"break", 0, 2, 0);
	trans[4][320]	= settr(989,0,0,585,585,"-end-", 0, 3500, 0);

	/* proctype 3: Egse_systemTc */

	trans[3] = (Trans **) emalloc(104*sizeof(Trans *));

	trans[3][1]	= settr(567,0,2,586,0,"(inited)", 1, 2, 0);
	trans[3][2]	= settr(568,0,100,587,587,"", 0, 2, 0);
	trans[3][101]	= settr(667,0,100,1,0,".(goto)", 0, 2, 0);
	T = trans[3][100] = settr(666,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(666,0,99,0,0,"DO", 0, 2, 0);
	T = trans[ 3][99] = settr(665,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(665,2,96,0,0,"ATOMIC", 1, 2, 0);
	T = trans[ 3][96] = settr(662,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(662,0,94,0,0,"sub-sequence", 1, 2, 0);
	T = trans[3][94] = settr(660,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(660,2,3,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(660,2,18,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(660,2,79,0,0,"IF", 1, 2, 0);
	trans[3][3]	= settr(569,2,16,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][16] = settr(582,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(582,0,4,0,0,"sub-sequence", 1, 2, 0);
	trans[3][4]	= settr(570,2,13,588,588,"Dummy_tmp = 0", 1, 2, 0);
	T = trans[3][13] = settr(579,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(579,2,12,0,0,"IF", 1, 2, 0);
	T = trans[ 3][12] = settr(578,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(578,0,5,0,0,"sub-sequence", 1, 2, 0);
	trans[3][5]	= settr(571,2,9,589,589,"Dummy_tmp = 0", 1, 2, 0);
	trans[3][10]	= settr(576,2,9,1,0,".(goto)", 1, 2, 0);
	T = trans[3][9] = settr(575,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(575,2,8,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(575,2,6,0,0,"DO", 1, 2, 0);
	trans[3][8]	= settr(574,2,98,590,590,"goto :b12", 1, 2, 0); /* m: 15 -> 0,98 */
	reached3[15] = 1;
	trans[3][6]	= settr(572,2,9,591,591,"((Dummy_tmp<0))", 1, 2, 0); /* m: 7 -> 9,0 */
	reached3[7] = 1;
	trans[3][7]	= settr(0,0,0,0,0,"Dummy_tmp = (Dummy_tmp+1)",0,0,0);
	trans[3][11]	= settr(577,2,15,1,0,"break", 1, 2, 0);
	trans[3][14]	= settr(580,2,15,1,0,".(goto)", 1, 2, 0); /* m: 15 -> 0,98 */
	reached3[15] = 1;
	trans[3][15]	= settr(581,2,98,592,592,"value.data.goToIdle = Dummy_tmp", 1, 2, 0); /* m: 17 -> 0,98 */
	reached3[17] = 1;
	trans[3][17]	= settr(0,0,0,0,0,"value.selection = 1",0,0,0);
	trans[3][95]	= settr(661,2,98,1,0,".(goto)", 1, 2, 0);
	trans[3][18]	= settr(584,2,77,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][77] = settr(643,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(643,0,47,0,0,"sub-sequence", 1, 2, 0);
	T = trans[ 3][47] = settr(613,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(613,0,19,0,0,"sub-sequence", 1, 2, 0);
	trans[3][19]	= settr(585,2,44,593,593,"OptimizedDataItem_tmp = 0", 1, 2, 0);
	T = trans[3][44] = settr(610,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(610,2,27,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(610,2,35,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(610,2,43,0,0,"IF", 1, 2, 0);
	T = trans[ 3][27] = settr(593,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(593,0,20,0,0,"sub-sequence", 1, 2, 0);
	trans[3][20]	= settr(586,2,24,594,594,"OptimizedDataItem_tmp = 10", 1, 2, 0);
	trans[3][25]	= settr(591,2,24,1,0,".(goto)", 1, 2, 0);
	T = trans[3][24] = settr(590,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(590,2,23,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(590,2,21,0,0,"DO", 1, 2, 0);
	trans[3][23]	= settr(589,2,76,595,595,"goto :b13", 1, 2, 0); /* m: 46 -> 0,76 */
	reached3[46] = 1;
	trans[3][21]	= settr(587,2,24,596,596,"((OptimizedDataItem_tmp<10))", 1, 2, 0); /* m: 22 -> 24,0 */
	reached3[22] = 1;
	trans[3][22]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][26]	= settr(592,2,46,1,0,"break", 1, 2, 0);
	trans[3][45]	= settr(611,2,46,1,0,".(goto)", 1, 2, 0); /* m: 46 -> 0,76 */
	reached3[46] = 1;
	T = trans[ 3][35] = settr(601,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(601,0,28,0,0,"sub-sequence", 1, 2, 0);
	trans[3][28]	= settr(594,2,32,597,597,"OptimizedDataItem_tmp = 50", 1, 2, 0);
	trans[3][33]	= settr(599,2,32,1,0,".(goto)", 1, 2, 0);
	T = trans[3][32] = settr(598,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(598,2,31,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(598,2,29,0,0,"DO", 1, 2, 0);
	trans[3][31]	= settr(597,2,76,598,598,"goto :b14", 1, 2, 0); /* m: 46 -> 0,76 */
	reached3[46] = 1;
	trans[3][29]	= settr(595,2,32,599,599,"((OptimizedDataItem_tmp<50))", 1, 2, 0); /* m: 30 -> 32,0 */
	reached3[30] = 1;
	trans[3][30]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][34]	= settr(600,2,46,1,0,"break", 1, 2, 0);
	T = trans[ 3][43] = settr(609,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(609,0,36,0,0,"sub-sequence", 1, 2, 0);
	trans[3][36]	= settr(602,2,40,600,600,"OptimizedDataItem_tmp = 100", 1, 2, 0);
	trans[3][41]	= settr(607,2,40,1,0,".(goto)", 1, 2, 0);
	T = trans[3][40] = settr(606,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(606,2,39,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(606,2,37,0,0,"DO", 1, 2, 0);
	trans[3][39]	= settr(605,2,76,601,601,"goto :b15", 1, 2, 0); /* m: 46 -> 0,76 */
	reached3[46] = 1;
	trans[3][37]	= settr(603,2,40,602,602,"((OptimizedDataItem_tmp<100))", 1, 2, 0); /* m: 38 -> 40,0 */
	reached3[38] = 1;
	trans[3][38]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][42]	= settr(608,2,46,1,0,"break", 1, 2, 0);
	trans[3][46]	= settr(612,2,76,603,603,"value.data.goToOperational.threshold1 = OptimizedDataItem_tmp", 1, 2, 0);
	T = trans[ 3][76] = settr(642,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(642,0,48,0,0,"sub-sequence", 1, 2, 0);
	trans[3][48]	= settr(614,2,73,604,604,"OptimizedDataItem_tmp = 0", 1, 2, 0);
	T = trans[3][73] = settr(639,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(639,2,56,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(639,2,64,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(639,2,72,0,0,"IF", 1, 2, 0);
	T = trans[ 3][56] = settr(622,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(622,0,49,0,0,"sub-sequence", 1, 2, 0);
	trans[3][49]	= settr(615,2,53,605,605,"OptimizedDataItem_tmp = 10", 1, 2, 0);
	trans[3][54]	= settr(620,2,53,1,0,".(goto)", 1, 2, 0);
	T = trans[3][53] = settr(619,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(619,2,52,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(619,2,50,0,0,"DO", 1, 2, 0);
	trans[3][52]	= settr(618,2,98,606,606,"goto :b16", 1, 2, 0); /* m: 75 -> 0,98 */
	reached3[75] = 1;
	trans[3][50]	= settr(616,2,53,607,607,"((OptimizedDataItem_tmp<10))", 1, 2, 0); /* m: 51 -> 53,0 */
	reached3[51] = 1;
	trans[3][51]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][55]	= settr(621,2,75,1,0,"break", 1, 2, 0);
	trans[3][74]	= settr(640,2,75,1,0,".(goto)", 1, 2, 0); /* m: 75 -> 0,98 */
	reached3[75] = 1;
	T = trans[ 3][64] = settr(630,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(630,0,57,0,0,"sub-sequence", 1, 2, 0);
	trans[3][57]	= settr(623,2,61,608,608,"OptimizedDataItem_tmp = 50", 1, 2, 0);
	trans[3][62]	= settr(628,2,61,1,0,".(goto)", 1, 2, 0);
	T = trans[3][61] = settr(627,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(627,2,60,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(627,2,58,0,0,"DO", 1, 2, 0);
	trans[3][60]	= settr(626,2,98,609,609,"goto :b17", 1, 2, 0); /* m: 75 -> 0,98 */
	reached3[75] = 1;
	trans[3][58]	= settr(624,2,61,610,610,"((OptimizedDataItem_tmp<50))", 1, 2, 0); /* m: 59 -> 61,0 */
	reached3[59] = 1;
	trans[3][59]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][63]	= settr(629,2,75,1,0,"break", 1, 2, 0);
	T = trans[ 3][72] = settr(638,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(638,0,65,0,0,"sub-sequence", 1, 2, 0);
	trans[3][65]	= settr(631,2,69,611,611,"OptimizedDataItem_tmp = 100", 1, 2, 0);
	trans[3][70]	= settr(636,2,69,1,0,".(goto)", 1, 2, 0);
	T = trans[3][69] = settr(635,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(635,2,68,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(635,2,66,0,0,"DO", 1, 2, 0);
	trans[3][68]	= settr(634,2,98,612,612,"goto :b18", 1, 2, 0); /* m: 75 -> 0,98 */
	reached3[75] = 1;
	trans[3][66]	= settr(632,2,69,613,613,"((OptimizedDataItem_tmp<100))", 1, 2, 0); /* m: 67 -> 69,0 */
	reached3[67] = 1;
	trans[3][67]	= settr(0,0,0,0,0,"OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)",0,0,0);
	trans[3][71]	= settr(637,2,75,1,0,"break", 1, 2, 0);
	trans[3][75]	= settr(641,2,98,614,614,"value.data.goToOperational.threshold2 = OptimizedDataItem_tmp", 1, 2, 0); /* m: 78 -> 0,98 */
	reached3[78] = 1;
	trans[3][78]	= settr(0,0,0,0,0,"value.selection = 2",0,0,0);
	trans[3][79]	= settr(645,2,92,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][92] = settr(658,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(658,0,80,0,0,"sub-sequence", 1, 2, 0);
	trans[3][80]	= settr(646,2,89,615,615,"Dummy_tmp = 0", 1, 2, 0);
	T = trans[3][89] = settr(655,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(655,2,88,0,0,"IF", 1, 2, 0);
	T = trans[ 3][88] = settr(654,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(654,0,81,0,0,"sub-sequence", 1, 2, 0);
	trans[3][81]	= settr(647,2,85,616,616,"Dummy_tmp = 0", 1, 2, 0);
	trans[3][86]	= settr(652,2,85,1,0,".(goto)", 1, 2, 0);
	T = trans[3][85] = settr(651,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(651,2,84,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(651,2,82,0,0,"DO", 1, 2, 0);
	trans[3][84]	= settr(650,2,98,617,617,"goto :b19", 1, 2, 0); /* m: 91 -> 0,98 */
	reached3[91] = 1;
	trans[3][82]	= settr(648,2,85,618,618,"((Dummy_tmp<0))", 1, 2, 0); /* m: 83 -> 85,0 */
	reached3[83] = 1;
	trans[3][83]	= settr(0,0,0,0,0,"Dummy_tmp = (Dummy_tmp+1)",0,0,0);
	trans[3][87]	= settr(653,2,91,1,0,"break", 1, 2, 0);
	trans[3][90]	= settr(656,2,91,1,0,".(goto)", 1, 2, 0); /* m: 91 -> 0,98 */
	reached3[91] = 1;
	trans[3][91]	= settr(657,2,98,619,619,"value.data.doReport = Dummy_tmp", 1, 2, 0); /* m: 93 -> 0,98 */
	reached3[93] = 1;
	trans[3][93]	= settr(0,0,0,0,0,"value.selection = 3",0,0,0);
	T = trans[ 3][98] = settr(664,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(664,0,97,0,0,"sub-sequence", 1, 2, 0);
	trans[3][97]	= settr(663,0,100,620,620,"Modemanager_systemTc_channel!value.data.goToIdle,value.data.goToOperational.threshold1,value.data.goToOperational.threshold2,value.data.doReport,value.selection", 1, 9, 0);
	trans[3][102]	= settr(668,0,103,1,0,"break", 0, 2, 0);
	trans[3][103]	= settr(669,0,0,621,621,"-end-", 0, 3500, 0);

	/* proctype 2: Egse_systemTm */

	trans[2] = (Trans **) emalloc(116*sizeof(Trans *));

	trans[2][1]	= settr(452,0,112,622,0,"(inited)", 1, 2, 0);
	trans[2][113]	= settr(564,0,112,1,0,".(goto)", 0, 2, 0);
	T = trans[2][112] = settr(563,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(563,0,111,0,0,"DO", 0, 2, 0);
	T = trans[ 2][111] = settr(562,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(562,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[2][2]	= settr(453,2,109,623,0,"(nempty(Egse_systemTm_channel))", 1, 2500, 0);
	T = trans[2][109] = settr(560,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(560,2,3,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(560,2,107,0,0,"IF", 1, 2500, 0);
	trans[2][3]	= settr(454,2,4,624,0,"(nempty(Egse_systemTm_channel))", 1, 2500, 0);
	trans[2][4]	= settr(455,2,6,625,625,"Egse_systemTm_channel?Egse_systemtm_signal_parameter.reading,Egse_systemtm_signal_parameter.status1,Egse_systemtm_signal_parameter.status2", 1, 2500, 0); /* m: 5 -> 6,0 */
	reached2[5] = 1;
	trans[2][5]	= settr(0,0,0,0,0,"Egse_systemtm_channel_used = 1",0,0,0);
	trans[2][6]	= settr(457,2,104,626,626,"Untitled_msc_lock?_", 1, 2500, 0);
	T = trans[ 2][104] = settr(555,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(555,0,102,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[2][102] = settr(553,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(553,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(553,2,47,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(553,2,100,0,0,"IF", 1, 2500, 0);
	trans[2][7]	= settr(458,2,46,627,0,"((global_state.untitled_msc.state==0))", 1, 2500, 0);
	T = trans[ 2][46] = settr(497,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(497,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][8]	= settr(459,2,43,628,628,"transition_id = 0", 1, 2500, 0); /* m: 9 -> 0,43 */
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
	trans[2][10]	= settr(461,2,105,629,629,"((transition_id==-(1)))", 1, 2500, 0);
	trans[2][11]	= settr(462,2,105,1,0,"goto :b9", 1, 2500, 0);
	trans[2][41]	= settr(492,2,42,1,0,".(goto)", 1, 2500, 0);
	trans[2][12]	= settr(463,2,42,630,630,"((transition_id==0))", 1, 2500, 0); /* m: 13 -> 42,0 */
	reached2[13] = 1;
	trans[2][13]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][14]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][15]	= settr(466,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][16]	= settr(467,2,23,631,631,"((transition_id==1))", 1, 2500, 0);
	T = trans[2][23] = settr(474,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(474,2,17,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(474,2,21,0,0,"IF", 1, 2500, 0);
	trans[2][17]	= settr(468,2,42,632,632,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 18 -> 42,0 */
	reached2[18] = 1;
	trans[2][18]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][19]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[2][20]	= settr(471,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][24]	= settr(475,2,25,1,0,".(goto)", 1, 2500, 0); /* m: 25 -> 0,42 */
	reached2[25] = 1;
	trans[2][21]	= settr(472,2,22,2,0,"else", 1, 2500, 0);
	trans[2][22]	= settr(473,2,42,633,633,"(1)", 1, 2500, 0); /* m: 25 -> 42,0 */
	reached2[25] = 1;
	trans[2][25]	= settr(476,2,42,634,634,"transition_id = -(1)", 1, 2500, 0); /* m: 26 -> 0,42 */
	reached2[26] = 1;
	trans[2][26]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][27]	= settr(478,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][28]	= settr(479,2,42,635,635,"((transition_id==2))", 1, 2500, 0); /* m: 29 -> 42,0 */
	reached2[29] = 1;
	trans[2][29]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][30]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][31]	= settr(482,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][32]	= settr(483,2,42,636,636,"((transition_id==3))", 1, 2500, 0); /* m: 33 -> 42,0 */
	reached2[33] = 1;
	trans[2][33]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][34]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[2][35]	= settr(486,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][36]	= settr(487,2,42,637,637,"((transition_id==4))", 1, 2500, 0); /* m: 37 -> 42,0 */
	reached2[37] = 1;
	trans[2][37]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][38]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[2][39]	= settr(490,2,42,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][42]	= settr(493,2,43,1,0,"(1)", 1, 2500, 0);
	trans[2][45]	= settr(496,2,105,1,0,"break", 1, 2500, 0);
	trans[2][103]	= settr(554,2,105,1,0,".(goto)", 1, 2500, 0);
	trans[2][47]	= settr(498,2,60,638,0,"((global_state.untitled_msc.state==2))", 1, 2500, 0);
	T = trans[ 2][60] = settr(511,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(511,0,51,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 2][51] = settr(502,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(502,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][48]	= settr(499,2,50,639,639,"Untitled_MSC_0_sig1_param0.reading = Egse_systemtm_signal_parameter.reading", 1, 2500, 0);
	T = trans[ 2][50] = settr(501,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(501,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][49]	= settr(500,2,55,640,0,"assert(((Untitled_MSC_0_sig1_param0.reading>=0)&&(Untitled_MSC_0_sig1_param0.reading<=255)))", 1, 2500, 0);
	T = trans[ 2][55] = settr(506,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(506,0,52,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][52]	= settr(503,2,54,641,641,"Untitled_MSC_0_sig1_param0.status1 = Egse_systemtm_signal_parameter.status1", 1, 2500, 0);
	T = trans[ 2][54] = settr(505,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(505,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][53]	= settr(504,2,59,642,0,"assert((((Untitled_MSC_0_sig1_param0.status1==0)||(Untitled_MSC_0_sig1_param0.status1==1))||(Untitled_MSC_0_sig1_param0.status1==2)))", 1, 2500, 0);
	T = trans[ 2][59] = settr(510,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(510,0,56,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][56]	= settr(507,2,58,643,643,"Untitled_MSC_0_sig1_param0.status2 = Egse_systemtm_signal_parameter.status2", 1, 2500, 0);
	T = trans[ 2][58] = settr(509,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(509,0,57,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][57]	= settr(508,2,99,644,0,"assert((((Untitled_MSC_0_sig1_param0.status2==0)||(Untitled_MSC_0_sig1_param0.status2==1))||(Untitled_MSC_0_sig1_param0.status2==2)))", 1, 2500, 0);
	T = trans[ 2][99] = settr(550,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(550,0,61,0,0,"sub-sequence", 1, 2500, 0);
	trans[2][61]	= settr(512,2,96,645,645,"transition_id = 0", 1, 2500, 0); /* m: 62 -> 0,96 */
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
	trans[2][63]	= settr(514,2,105,646,646,"((transition_id==-(1)))", 1, 2500, 0);
	trans[2][64]	= settr(515,2,105,1,0,"goto :b10", 1, 2500, 0);
	trans[2][94]	= settr(545,2,95,1,0,".(goto)", 1, 2500, 0);
	trans[2][65]	= settr(516,2,95,647,647,"((transition_id==0))", 1, 2500, 0); /* m: 66 -> 95,0 */
	reached2[66] = 1;
	trans[2][66]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][67]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][68]	= settr(519,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][69]	= settr(520,2,76,648,648,"((transition_id==1))", 1, 2500, 0);
	T = trans[2][76] = settr(527,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(527,2,70,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(527,2,74,0,0,"IF", 1, 2500, 0);
	trans[2][70]	= settr(521,2,95,649,649,"((Untitled_MSC_0_sig0_param0.selection==3))", 1, 2500, 0); /* m: 71 -> 95,0 */
	reached2[71] = 1;
	trans[2][71]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][72]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 2",0,0,0);
	trans[2][73]	= settr(524,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][77]	= settr(528,2,78,1,0,".(goto)", 1, 2500, 0); /* m: 78 -> 0,95 */
	reached2[78] = 1;
	trans[2][74]	= settr(525,2,75,2,0,"else", 1, 2500, 0);
	trans[2][75]	= settr(526,2,95,650,650,"(1)", 1, 2500, 0); /* m: 78 -> 95,0 */
	reached2[78] = 1;
	trans[2][78]	= settr(529,2,95,651,651,"transition_id = -(1)", 1, 2500, 0); /* m: 79 -> 0,95 */
	reached2[79] = 1;
	trans[2][79]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][80]	= settr(531,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][81]	= settr(532,2,95,652,652,"((transition_id==2))", 1, 2500, 0); /* m: 82 -> 95,0 */
	reached2[82] = 1;
	trans[2][82]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][83]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 0",0,0,0);
	trans[2][84]	= settr(535,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][85]	= settr(536,2,95,653,653,"((transition_id==3))", 1, 2500, 0); /* m: 86 -> 95,0 */
	reached2[86] = 1;
	trans[2][86]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][87]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 1",0,0,0);
	trans[2][88]	= settr(539,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][89]	= settr(540,2,95,654,654,"((transition_id==4))", 1, 2500, 0); /* m: 90 -> 95,0 */
	reached2[90] = 1;
	trans[2][90]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[2][91]	= settr(0,0,0,0,0,"global_state.untitled_msc.state = 3",0,0,0);
	trans[2][92]	= settr(543,2,95,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[2][95]	= settr(546,2,96,1,0,"(1)", 1, 2500, 0);
	trans[2][98]	= settr(549,2,105,1,0,"break", 1, 2500, 0);
	trans[2][100]	= settr(551,2,101,2,0,"else", 1, 2500, 0);
	trans[2][101]	= settr(552,2,105,1,0,"(1)", 1, 2500, 0);
	trans[2][105]	= settr(556,2,109,655,655,"Untitled_msc_lock!1", 1, 2500, 0);
	trans[2][106]	= settr(557,2,109,1,0,"goto Egse_systemtm_loop", 1, 2500, 0);
	trans[2][110]	= settr(561,0,112,1,0,".(goto)", 1, 2500, 0);
	trans[2][107]	= settr(558,2,108,656,0,"(empty(Egse_systemTm_channel))", 1, 2500, 0);
	trans[2][108]	= settr(559,0,112,657,657,"(1)", 1, 2500, 0); /* m: 110 -> 112,0 */
	reached2[110] = 1;
	trans[2][114]	= settr(565,0,115,1,0,"break", 0, 2, 0);
	trans[2][115]	= settr(566,0,0,658,658,"-end-", 0, 3500, 0);

	/* proctype 1: Actuator2_actuatorTc */

	trans[1] = (Trans **) emalloc(227*sizeof(Trans *));

	trans[1][1]	= settr(226,0,223,659,0,"(inited)", 1, 2, 0);
	trans[1][224]	= settr(449,0,223,1,0,".(goto)", 0, 2, 0);
	T = trans[1][223] = settr(448,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(448,0,222,0,0,"DO", 0, 2, 0);
	T = trans[ 1][222] = settr(447,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(447,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[1][2]	= settr(227,2,3,660,0,"(nempty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][3]	= settr(228,2,219,661,661,"Actuator2_lock?_", 1, 2500, 0);
	T = trans[1][219] = settr(444,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(444,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(444,2,217,0,0,"IF", 1, 2500, 0);
	trans[1][4]	= settr(229,2,5,662,0,"(nempty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][5]	= settr(230,2,215,663,663,"Actuator2_actuatorTc_channel?Actuator2_actuatortc_signal_parameter", 1, 2500, 0); /* m: 6 -> 215,0 */
	reached1[6] = 1;
	trans[1][6]	= settr(0,0,0,0,0,"Actuator2_actuatortc_channel_used = 1",0,0,0);
	T = trans[ 1][215] = settr(440,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(440,0,213,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[1][213] = settr(438,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,75,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,143,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(438,2,211,0,0,"IF", 1, 2500, 0);
	trans[1][7]	= settr(232,2,11,664,0,"((global_state.actuator2.state==0))", 1, 2500, 0);
	T = trans[ 1][11] = settr(236,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(236,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][8]	= settr(233,2,10,665,665,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][10] = settr(235,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(235,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][9]	= settr(234,2,74,666,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][74] = settr(299,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(299,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][12]	= settr(237,2,71,667,667,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,71 */
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
	trans[1][14]	= settr(239,2,219,668,668,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][15]	= settr(240,2,219,1,0,"goto :b5", 1, 2500, 0);
	trans[1][69]	= settr(294,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][16]	= settr(241,2,70,669,669,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 70,0 */
	reached1[17] = 1;
	trans[1][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][18]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][19]	= settr(244,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][20]	= settr(245,2,22,670,670,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][22] = settr(247,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(247,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][21]	= settr(246,2,38,1,0,"(1)", 1, 2500, 0);
	T = trans[1][38] = settr(263,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(263,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(263,2,29,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(263,2,35,0,0,"IF", 1, 2500, 0);
	trans[1][23]	= settr(248,2,25,671,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][25] = settr(250,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(250,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][24]	= settr(249,2,70,672,672,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 26 -> 70,0 */
	reached1[26] = 1;
	trans[1][26]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][27]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][28]	= settr(253,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][39]	= settr(264,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][29]	= settr(254,2,31,673,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][31] = settr(256,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(256,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][30]	= settr(255,2,70,674,674,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 32 -> 70,0 */
	reached1[32] = 1;
	trans[1][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][33]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][34]	= settr(259,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][35]	= settr(260,2,36,2,0,"else", 1, 2500, 0);
	trans[1][36]	= settr(261,2,70,675,675,"transition_id = -(1)", 1, 2500, 0);
	trans[1][37]	= settr(262,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][40]	= settr(265,2,42,676,676,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][42] = settr(267,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(267,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][41]	= settr(266,2,52,1,0,"(1)", 1, 2500, 0);
	T = trans[1][52] = settr(277,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(277,2,43,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(277,2,49,0,0,"IF", 1, 2500, 0);
	trans[1][43]	= settr(268,2,45,677,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][45] = settr(270,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(270,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][44]	= settr(269,2,70,678,678,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 46 -> 70,0 */
	reached1[46] = 1;
	trans[1][46]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][47]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][48]	= settr(273,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][53]	= settr(278,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][49]	= settr(274,2,50,2,0,"else", 1, 2500, 0);
	trans[1][50]	= settr(275,2,70,679,679,"transition_id = -(1)", 1, 2500, 0);
	trans[1][51]	= settr(276,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][54]	= settr(279,2,56,680,680,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][56] = settr(281,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(281,0,55,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][55]	= settr(280,2,66,1,0,"(1)", 1, 2500, 0);
	T = trans[1][66] = settr(291,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(291,2,57,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(291,2,63,0,0,"IF", 1, 2500, 0);
	trans[1][57]	= settr(282,2,59,681,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][59] = settr(284,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(284,0,58,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][58]	= settr(283,2,70,682,682,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 60 -> 70,0 */
	reached1[60] = 1;
	trans[1][60]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][61]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][62]	= settr(287,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][67]	= settr(292,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][63]	= settr(288,2,64,2,0,"else", 1, 2500, 0);
	trans[1][64]	= settr(289,2,70,683,683,"transition_id = -(1)", 1, 2500, 0);
	trans[1][65]	= settr(290,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][70]	= settr(295,2,71,1,0,"(1)", 1, 2500, 0);
	trans[1][73]	= settr(298,2,219,1,0,"break", 1, 2500, 0);
	trans[1][214]	= settr(439,2,219,1,0,".(goto)", 1, 2500, 0);
	trans[1][75]	= settr(300,2,79,684,0,"((global_state.actuator2.state==1))", 1, 2500, 0);
	T = trans[ 1][79] = settr(304,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(304,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][76]	= settr(301,2,78,685,685,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][78] = settr(303,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(303,0,77,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][77]	= settr(302,2,142,686,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][142] = settr(367,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(367,0,80,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][80]	= settr(305,2,139,687,687,"transition_id = 0", 1, 2500, 0); /* m: 81 -> 0,139 */
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
	trans[1][82]	= settr(307,2,219,688,688,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][83]	= settr(308,2,219,1,0,"goto :b6", 1, 2500, 0);
	trans[1][137]	= settr(362,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][84]	= settr(309,2,138,689,689,"((transition_id==0))", 1, 2500, 0); /* m: 85 -> 138,0 */
	reached1[85] = 1;
	trans[1][85]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][86]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][87]	= settr(312,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][88]	= settr(313,2,90,690,690,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][90] = settr(315,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(315,0,89,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][89]	= settr(314,2,106,1,0,"(1)", 1, 2500, 0);
	T = trans[1][106] = settr(331,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(331,2,91,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(331,2,97,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(331,2,103,0,0,"IF", 1, 2500, 0);
	trans[1][91]	= settr(316,2,93,691,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][93] = settr(318,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(318,0,92,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][92]	= settr(317,2,138,692,692,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 94 -> 138,0 */
	reached1[94] = 1;
	trans[1][94]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][95]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][96]	= settr(321,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][107]	= settr(332,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][97]	= settr(322,2,99,693,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][99] = settr(324,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(324,0,98,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][98]	= settr(323,2,138,694,694,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 100 -> 138,0 */
	reached1[100] = 1;
	trans[1][100]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][101]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][102]	= settr(327,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][103]	= settr(328,2,104,2,0,"else", 1, 2500, 0);
	trans[1][104]	= settr(329,2,138,695,695,"transition_id = -(1)", 1, 2500, 0);
	trans[1][105]	= settr(330,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][108]	= settr(333,2,110,696,696,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][110] = settr(335,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(335,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][109]	= settr(334,2,120,1,0,"(1)", 1, 2500, 0);
	T = trans[1][120] = settr(345,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(345,2,111,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(345,2,117,0,0,"IF", 1, 2500, 0);
	trans[1][111]	= settr(336,2,113,697,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][113] = settr(338,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(338,0,112,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][112]	= settr(337,2,138,698,698,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 114 -> 138,0 */
	reached1[114] = 1;
	trans[1][114]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][115]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][116]	= settr(341,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][121]	= settr(346,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][117]	= settr(342,2,118,2,0,"else", 1, 2500, 0);
	trans[1][118]	= settr(343,2,138,699,699,"transition_id = -(1)", 1, 2500, 0);
	trans[1][119]	= settr(344,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][122]	= settr(347,2,124,700,700,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][124] = settr(349,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(349,0,123,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][123]	= settr(348,2,134,1,0,"(1)", 1, 2500, 0);
	T = trans[1][134] = settr(359,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(359,2,125,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(359,2,131,0,0,"IF", 1, 2500, 0);
	trans[1][125]	= settr(350,2,127,701,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][127] = settr(352,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(352,0,126,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][126]	= settr(351,2,138,702,702,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 128 -> 138,0 */
	reached1[128] = 1;
	trans[1][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][129]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][130]	= settr(355,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][135]	= settr(360,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][131]	= settr(356,2,132,2,0,"else", 1, 2500, 0);
	trans[1][132]	= settr(357,2,138,703,703,"transition_id = -(1)", 1, 2500, 0);
	trans[1][133]	= settr(358,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][138]	= settr(363,2,139,1,0,"(1)", 1, 2500, 0);
	trans[1][141]	= settr(366,2,219,1,0,"break", 1, 2500, 0);
	trans[1][143]	= settr(368,2,147,704,0,"((global_state.actuator2.state==2))", 1, 2500, 0);
	T = trans[ 1][147] = settr(372,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(372,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][144]	= settr(369,2,146,705,705,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][146] = settr(371,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(371,0,145,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][145]	= settr(370,2,210,706,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][210] = settr(435,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(435,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][148]	= settr(373,2,207,707,707,"transition_id = 0", 1, 2500, 0); /* m: 149 -> 0,207 */
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
	trans[1][150]	= settr(375,2,219,708,708,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][151]	= settr(376,2,219,1,0,"goto :b7", 1, 2500, 0);
	trans[1][205]	= settr(430,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][152]	= settr(377,2,206,709,709,"((transition_id==0))", 1, 2500, 0); /* m: 153 -> 206,0 */
	reached1[153] = 1;
	trans[1][153]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][154]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][155]	= settr(380,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][156]	= settr(381,2,158,710,710,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][158] = settr(383,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(383,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][157]	= settr(382,2,174,1,0,"(1)", 1, 2500, 0);
	T = trans[1][174] = settr(399,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(399,2,159,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(399,2,165,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(399,2,171,0,0,"IF", 1, 2500, 0);
	trans[1][159]	= settr(384,2,161,711,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][161] = settr(386,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(386,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][160]	= settr(385,2,206,712,712,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 162 -> 206,0 */
	reached1[162] = 1;
	trans[1][162]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][163]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][164]	= settr(389,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][175]	= settr(400,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][165]	= settr(390,2,167,713,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][167] = settr(392,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(392,0,166,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][166]	= settr(391,2,206,714,714,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 168 -> 206,0 */
	reached1[168] = 1;
	trans[1][168]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][169]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][170]	= settr(395,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][171]	= settr(396,2,172,2,0,"else", 1, 2500, 0);
	trans[1][172]	= settr(397,2,206,715,715,"transition_id = -(1)", 1, 2500, 0);
	trans[1][173]	= settr(398,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][176]	= settr(401,2,178,716,716,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][178] = settr(403,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(403,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][177]	= settr(402,2,188,1,0,"(1)", 1, 2500, 0);
	T = trans[1][188] = settr(413,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(413,2,179,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(413,2,185,0,0,"IF", 1, 2500, 0);
	trans[1][179]	= settr(404,2,181,717,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][181] = settr(406,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(406,0,180,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][180]	= settr(405,2,206,718,718,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 182 -> 206,0 */
	reached1[182] = 1;
	trans[1][182]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][183]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][184]	= settr(409,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][189]	= settr(414,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][185]	= settr(410,2,186,2,0,"else", 1, 2500, 0);
	trans[1][186]	= settr(411,2,206,719,719,"transition_id = -(1)", 1, 2500, 0);
	trans[1][187]	= settr(412,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][190]	= settr(415,2,192,720,720,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][192] = settr(417,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(417,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][191]	= settr(416,2,202,1,0,"(1)", 1, 2500, 0);
	T = trans[1][202] = settr(427,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(427,2,193,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(427,2,199,0,0,"IF", 1, 2500, 0);
	trans[1][193]	= settr(418,2,195,721,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][195] = settr(420,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(420,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][194]	= settr(419,2,206,722,722,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 196 -> 206,0 */
	reached1[196] = 1;
	trans[1][196]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][197]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][198]	= settr(423,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][203]	= settr(428,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][199]	= settr(424,2,200,2,0,"else", 1, 2500, 0);
	trans[1][200]	= settr(425,2,206,723,723,"transition_id = -(1)", 1, 2500, 0);
	trans[1][201]	= settr(426,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][206]	= settr(431,2,207,1,0,"(1)", 1, 2500, 0);
	trans[1][209]	= settr(434,2,219,1,0,"break", 1, 2500, 0);
	trans[1][211]	= settr(436,2,212,2,0,"else", 1, 2500, 0);
	trans[1][212]	= settr(437,2,219,1,0,"(1)", 1, 2500, 0);
	trans[1][216]	= settr(441,2,219,1,0,"goto Actuator2_actuatortc_loop", 1, 2500, 0);
	trans[1][220]	= settr(445,2,221,1,0,".(goto)", 1, 2500, 0);
	trans[1][217]	= settr(442,2,218,724,0,"(empty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][218]	= settr(443,2,221,725,725,"(1)", 1, 2500, 0); /* m: 220 -> 221,0 */
	reached1[220] = 1;
	trans[1][221]	= settr(446,0,223,726,726,"Actuator2_lock!1", 1, 2500, 0);
	trans[1][225]	= settr(450,0,226,1,0,"break", 0, 2, 0);
	trans[1][226]	= settr(451,0,0,727,727,"-end-", 0, 3500, 0);

	/* proctype 0: Actuator1_actuatorTc */

	trans[0] = (Trans **) emalloc(227*sizeof(Trans *));

	trans[0][1]	= settr(0,0,223,728,0,"(inited)", 1, 2, 0);
	trans[0][224]	= settr(223,0,223,1,0,".(goto)", 0, 2, 0);
	T = trans[0][223] = settr(222,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(222,0,222,0,0,"DO", 0, 2, 0);
	T = trans[ 0][222] = settr(221,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(221,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[0][2]	= settr(1,2,3,729,0,"(nempty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][3]	= settr(2,2,219,730,730,"Actuator1_lock?_", 1, 2500, 0);
	T = trans[0][219] = settr(218,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(218,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(218,2,217,0,0,"IF", 1, 2500, 0);
	trans[0][4]	= settr(3,2,5,731,0,"(nempty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][5]	= settr(4,2,215,732,732,"Actuator1_actuatorTc_channel?Actuator1_actuatortc_signal_parameter", 1, 2500, 0); /* m: 6 -> 215,0 */
	reached0[6] = 1;
	trans[0][6]	= settr(0,0,0,0,0,"Actuator1_actuatortc_channel_used = 1",0,0,0);
	T = trans[ 0][215] = settr(214,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(214,0,213,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[0][213] = settr(212,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,75,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,143,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(212,2,211,0,0,"IF", 1, 2500, 0);
	trans[0][7]	= settr(6,2,11,733,0,"((global_state.actuator1.state==0))", 1, 2500, 0);
	T = trans[ 0][11] = settr(10,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(10,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][8]	= settr(7,2,10,734,734,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][10] = settr(9,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(9,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][9]	= settr(8,2,74,735,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][74] = settr(73,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(73,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][12]	= settr(11,2,71,736,736,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,71 */
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
	trans[0][14]	= settr(13,2,219,737,737,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][15]	= settr(14,2,219,1,0,"goto :b1", 1, 2500, 0);
	trans[0][69]	= settr(68,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][16]	= settr(15,2,70,738,738,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 70,0 */
	reached0[17] = 1;
	trans[0][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][18]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][19]	= settr(18,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][20]	= settr(19,2,22,739,739,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][22] = settr(21,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(21,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][21]	= settr(20,2,38,1,0,"(1)", 1, 2500, 0);
	T = trans[0][38] = settr(37,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(37,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(37,2,29,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(37,2,35,0,0,"IF", 1, 2500, 0);
	trans[0][23]	= settr(22,2,25,740,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][25] = settr(24,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(24,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][24]	= settr(23,2,70,741,741,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 26 -> 70,0 */
	reached0[26] = 1;
	trans[0][26]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][27]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][28]	= settr(27,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][39]	= settr(38,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][29]	= settr(28,2,31,742,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][31] = settr(30,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(30,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][30]	= settr(29,2,70,743,743,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 32 -> 70,0 */
	reached0[32] = 1;
	trans[0][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][33]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][34]	= settr(33,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][35]	= settr(34,2,36,2,0,"else", 1, 2500, 0);
	trans[0][36]	= settr(35,2,70,744,744,"transition_id = -(1)", 1, 2500, 0);
	trans[0][37]	= settr(36,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][40]	= settr(39,2,42,745,745,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][42] = settr(41,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(41,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][41]	= settr(40,2,52,1,0,"(1)", 1, 2500, 0);
	T = trans[0][52] = settr(51,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(51,2,43,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(51,2,49,0,0,"IF", 1, 2500, 0);
	trans[0][43]	= settr(42,2,45,746,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][45] = settr(44,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(44,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][44]	= settr(43,2,70,747,747,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 46 -> 70,0 */
	reached0[46] = 1;
	trans[0][46]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][47]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][48]	= settr(47,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][53]	= settr(52,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][49]	= settr(48,2,50,2,0,"else", 1, 2500, 0);
	trans[0][50]	= settr(49,2,70,748,748,"transition_id = -(1)", 1, 2500, 0);
	trans[0][51]	= settr(50,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][54]	= settr(53,2,56,749,749,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][56] = settr(55,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(55,0,55,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][55]	= settr(54,2,66,1,0,"(1)", 1, 2500, 0);
	T = trans[0][66] = settr(65,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(65,2,57,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(65,2,63,0,0,"IF", 1, 2500, 0);
	trans[0][57]	= settr(56,2,59,750,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][59] = settr(58,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(58,0,58,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][58]	= settr(57,2,70,751,751,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 60 -> 70,0 */
	reached0[60] = 1;
	trans[0][60]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][61]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][62]	= settr(61,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][67]	= settr(66,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][63]	= settr(62,2,64,2,0,"else", 1, 2500, 0);
	trans[0][64]	= settr(63,2,70,752,752,"transition_id = -(1)", 1, 2500, 0);
	trans[0][65]	= settr(64,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][70]	= settr(69,2,71,1,0,"(1)", 1, 2500, 0);
	trans[0][73]	= settr(72,2,219,1,0,"break", 1, 2500, 0);
	trans[0][214]	= settr(213,2,219,1,0,".(goto)", 1, 2500, 0);
	trans[0][75]	= settr(74,2,79,753,0,"((global_state.actuator1.state==1))", 1, 2500, 0);
	T = trans[ 0][79] = settr(78,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(78,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][76]	= settr(75,2,78,754,754,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][78] = settr(77,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(77,0,77,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][77]	= settr(76,2,142,755,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][142] = settr(141,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(141,0,80,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][80]	= settr(79,2,139,756,756,"transition_id = 0", 1, 2500, 0); /* m: 81 -> 0,139 */
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
	trans[0][82]	= settr(81,2,219,757,757,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][83]	= settr(82,2,219,1,0,"goto :b2", 1, 2500, 0);
	trans[0][137]	= settr(136,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][84]	= settr(83,2,138,758,758,"((transition_id==0))", 1, 2500, 0); /* m: 85 -> 138,0 */
	reached0[85] = 1;
	trans[0][85]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][86]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][87]	= settr(86,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][88]	= settr(87,2,90,759,759,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][90] = settr(89,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(89,0,89,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][89]	= settr(88,2,106,1,0,"(1)", 1, 2500, 0);
	T = trans[0][106] = settr(105,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(105,2,91,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(105,2,97,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(105,2,103,0,0,"IF", 1, 2500, 0);
	trans[0][91]	= settr(90,2,93,760,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][93] = settr(92,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(92,0,92,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][92]	= settr(91,2,138,761,761,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 94 -> 138,0 */
	reached0[94] = 1;
	trans[0][94]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][95]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][96]	= settr(95,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][107]	= settr(106,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][97]	= settr(96,2,99,762,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][99] = settr(98,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(98,0,98,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][98]	= settr(97,2,138,763,763,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 100 -> 138,0 */
	reached0[100] = 1;
	trans[0][100]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][101]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][102]	= settr(101,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][103]	= settr(102,2,104,2,0,"else", 1, 2500, 0);
	trans[0][104]	= settr(103,2,138,764,764,"transition_id = -(1)", 1, 2500, 0);
	trans[0][105]	= settr(104,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][108]	= settr(107,2,110,765,765,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][110] = settr(109,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(109,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][109]	= settr(108,2,120,1,0,"(1)", 1, 2500, 0);
	T = trans[0][120] = settr(119,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(119,2,111,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(119,2,117,0,0,"IF", 1, 2500, 0);
	trans[0][111]	= settr(110,2,113,766,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][113] = settr(112,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(112,0,112,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][112]	= settr(111,2,138,767,767,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 114 -> 138,0 */
	reached0[114] = 1;
	trans[0][114]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][115]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][116]	= settr(115,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][121]	= settr(120,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][117]	= settr(116,2,118,2,0,"else", 1, 2500, 0);
	trans[0][118]	= settr(117,2,138,768,768,"transition_id = -(1)", 1, 2500, 0);
	trans[0][119]	= settr(118,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][122]	= settr(121,2,124,769,769,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][124] = settr(123,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(123,0,123,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][123]	= settr(122,2,134,1,0,"(1)", 1, 2500, 0);
	T = trans[0][134] = settr(133,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(133,2,125,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(133,2,131,0,0,"IF", 1, 2500, 0);
	trans[0][125]	= settr(124,2,127,770,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][127] = settr(126,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(126,0,126,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][126]	= settr(125,2,138,771,771,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 128 -> 138,0 */
	reached0[128] = 1;
	trans[0][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][129]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][130]	= settr(129,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][135]	= settr(134,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][131]	= settr(130,2,132,2,0,"else", 1, 2500, 0);
	trans[0][132]	= settr(131,2,138,772,772,"transition_id = -(1)", 1, 2500, 0);
	trans[0][133]	= settr(132,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][138]	= settr(137,2,139,1,0,"(1)", 1, 2500, 0);
	trans[0][141]	= settr(140,2,219,1,0,"break", 1, 2500, 0);
	trans[0][143]	= settr(142,2,147,773,0,"((global_state.actuator1.state==2))", 1, 2500, 0);
	T = trans[ 0][147] = settr(146,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(146,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][144]	= settr(143,2,146,774,774,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][146] = settr(145,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(145,0,145,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][145]	= settr(144,2,210,775,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][210] = settr(209,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(209,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][148]	= settr(147,2,207,776,776,"transition_id = 0", 1, 2500, 0); /* m: 149 -> 0,207 */
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
	trans[0][150]	= settr(149,2,219,777,777,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][151]	= settr(150,2,219,1,0,"goto :b3", 1, 2500, 0);
	trans[0][205]	= settr(204,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][152]	= settr(151,2,206,778,778,"((transition_id==0))", 1, 2500, 0); /* m: 153 -> 206,0 */
	reached0[153] = 1;
	trans[0][153]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][154]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][155]	= settr(154,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][156]	= settr(155,2,158,779,779,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][158] = settr(157,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(157,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][157]	= settr(156,2,174,1,0,"(1)", 1, 2500, 0);
	T = trans[0][174] = settr(173,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(173,2,159,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(173,2,165,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(173,2,171,0,0,"IF", 1, 2500, 0);
	trans[0][159]	= settr(158,2,161,780,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][161] = settr(160,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(160,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][160]	= settr(159,2,206,781,781,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 162 -> 206,0 */
	reached0[162] = 1;
	trans[0][162]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][163]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][164]	= settr(163,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][175]	= settr(174,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][165]	= settr(164,2,167,782,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][167] = settr(166,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(166,0,166,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][166]	= settr(165,2,206,783,783,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 168 -> 206,0 */
	reached0[168] = 1;
	trans[0][168]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][169]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][170]	= settr(169,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][171]	= settr(170,2,172,2,0,"else", 1, 2500, 0);
	trans[0][172]	= settr(171,2,206,784,784,"transition_id = -(1)", 1, 2500, 0);
	trans[0][173]	= settr(172,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][176]	= settr(175,2,178,785,785,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][178] = settr(177,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(177,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][177]	= settr(176,2,188,1,0,"(1)", 1, 2500, 0);
	T = trans[0][188] = settr(187,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(187,2,179,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(187,2,185,0,0,"IF", 1, 2500, 0);
	trans[0][179]	= settr(178,2,181,786,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][181] = settr(180,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(180,0,180,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][180]	= settr(179,2,206,787,787,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 182 -> 206,0 */
	reached0[182] = 1;
	trans[0][182]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][183]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][184]	= settr(183,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][189]	= settr(188,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][185]	= settr(184,2,186,2,0,"else", 1, 2500, 0);
	trans[0][186]	= settr(185,2,206,788,788,"transition_id = -(1)", 1, 2500, 0);
	trans[0][187]	= settr(186,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][190]	= settr(189,2,192,789,789,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][192] = settr(191,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(191,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][191]	= settr(190,2,202,1,0,"(1)", 1, 2500, 0);
	T = trans[0][202] = settr(201,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(201,2,193,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(201,2,199,0,0,"IF", 1, 2500, 0);
	trans[0][193]	= settr(192,2,195,790,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][195] = settr(194,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(194,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][194]	= settr(193,2,206,791,791,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 196 -> 206,0 */
	reached0[196] = 1;
	trans[0][196]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][197]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][198]	= settr(197,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][203]	= settr(202,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][199]	= settr(198,2,200,2,0,"else", 1, 2500, 0);
	trans[0][200]	= settr(199,2,206,792,792,"transition_id = -(1)", 1, 2500, 0);
	trans[0][201]	= settr(200,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][206]	= settr(205,2,207,1,0,"(1)", 1, 2500, 0);
	trans[0][209]	= settr(208,2,219,1,0,"break", 1, 2500, 0);
	trans[0][211]	= settr(210,2,212,2,0,"else", 1, 2500, 0);
	trans[0][212]	= settr(211,2,219,1,0,"(1)", 1, 2500, 0);
	trans[0][216]	= settr(215,2,219,1,0,"goto Actuator1_actuatortc_loop", 1, 2500, 0);
	trans[0][220]	= settr(219,2,221,1,0,".(goto)", 1, 2500, 0);
	trans[0][217]	= settr(216,2,218,793,0,"(empty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][218]	= settr(217,2,221,794,794,"(1)", 1, 2500, 0); /* m: 220 -> 221,0 */
	reached0[220] = 1;
	trans[0][221]	= settr(220,0,223,795,795,"Actuator1_lock!1", 1, 2500, 0);
	trans[0][225]	= settr(224,0,226,1,0,"break", 0, 2, 0);
	trans[0][226]	= settr(225,0,0,796,796,"-end-", 0, 3500, 0);
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
