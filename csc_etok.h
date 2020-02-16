
/*
int parser (char const ** p, int * v, unsigned n, struct main_kv * kv)
{
	char const * a; //Token start
	int r; //Row line
	int c; //Column
	int tok = 0;
	char buf [100];
	//int * q = v;//debug usage
	int * e = v + n - 1; //End pointer
	int * w = v; //Count pointer
	int i;
	v++;
	while (1)
	{
		if (v == e) {break;}
		tok = tok_next (&a, p, &r, &c);
		if (tok == 0) {break;}
		memset (buf, 0, 100);
		memcpy (buf, a, MIN ((size_t)(p[0]-a), sizeof (buf)-1));
		switch (tok)
		{
		case TOK_ID:
			i = main_kv_find_index (kv, buf);
			v[0] = (i == -1) ? 0 : kv->value [i];
			w[0]++;
			v++;
			//printf ("%-20s : %i\n", buf, main_strval_find_value (strval, buf));
			break;

		case TOK_DIGIT:
			//v[0] = str_to_umax_ab (a, p[0], 10);
			v[0] = str_to_umax_ab_adabase (a, p[0]);
			w[0]++;
			v++;
			//printf ("%-20s : %ju\n", buf, str_to_umax_ab (a, p[0], 10));
			break;

		case ';':
			w = v;
			v++;
			break;
		}
	}
	v[0] = 0;
	v++;
	return e - v;
}
*/

/*
Number of elements in sub-arrays is encoded as an element.
Any value including 0 of content element (e) is allowed.
Count element (n1=1) says that there are 1 elements forward.
Count element (n3=3) says that there are 3 elements forward.
Count element (n0=0) says that this is the end of array.
(n1 e1 n3 e1 e2 e3 n0)
*/
/*
void print_ca (int * v, unsigned n)
{
	int * e = v + n;
	while (1)
	{
		//v0     : Number of sub-elements. v0 = 0 <=> end of array.
		//v1     : Should be the command id. Unrecognized command should be allowed.
		//v2..vn : Optional. Should probably be the parameters of the command.
		if (v[0] == 0){break;}
		if (v >= e){break;}
		//for (int i = 0; i < v[0]+1; ++i){printf ("%04x ", v[i]);}printf ("\n");
		switch (v[1])
		{
		case CMD_SLEEP:
			//ASSERT (w[0] == 2);
			printf ("SLEEP   %-20i\n", v[2]);
			break;
		case CMD_ASSIGN:
			printf ("ASSIGN  %-20s 0x%08x\n", od_str_get ((enum odref) v[2]), v[3]);
			break;
		default:
			printf ("UNKNOWN ");
			for (int i = 2; i < v[0]+1; ++i){printf ("%04i ", v[i]);}
			printf ("\n");
			break;
		}
		v += v[0] + 1;
		//if (v <= w){continue;}
	}
}
*/