#include <iostream>
#include <math.h>
#include <cstdlib> // ugh system

using namespace std;

main() {
	
	// dbug				Control de mensajes debug
	bool dbug=false;
	
	// VARIABLES DE CONTROL DE FLUJO
	// err_skill		true si no hay valor en ft o es equivocado
	// err_perc			true si no hay valor en percent o es equivocado
	// continue_calc	Continuar calculando y reiniciar el programa
	// usr_cont			Variable char para preguntar al usuario si quiere seguir
	bool err_skill, err_perc, continue_calc;
	char usr_cont;
	
	// Ok. Primero voy a crear un arreglo con la cantidad de xp que requiere cada skill.
	// Supuestamente unsigned long int permite numeros hasta 18446744073709551615. En WOT hay 25 skills/perks.
	// la cualificacion principal requiere 105030 xp
	// trabajamos con maximo 14 skills pq c++ trunca los valores de skills mas altos :(
	
	// n 		variable de control para ciclos for
	// mainq 	es la constante para calcular los valores de cada skill
	// ft		skills entrenados al 100%. Se le pregunta al usuario.
	// percent	porcentaje del skill parcialmente entrenado. Se le pregunta al usuario.
	// ft_reset	skills entrenados al 100% luego del reset.
	int n, mainq = 105030, ft=0, percent=0, menu, ft_reset;
	
	// xpartial	Es la XP requerida para pasar de un porcentaje x a x+1. Debe calcularse como float.
	float xpartial;
	
	// skills	array con la xp que requiere cada skill. 14 velores.
	// totalxp	array con el acumulado total de xp por n° de skills. 14 velores.
	// acum		acumulador que ayuda a asignar valores a totalxp. se puede usar en otros casos
	// fullxp	Es el valor de toda la XP. se obtiene sumando acum y un valor de totalxp
	unsigned long int skills[14], totalxp[14], acum=0, fullxp;
	
	cout << fixed;
	
	// Lleno el array skills con los valores que se requieren.
	for ( n=0; n<=14; n++ ) {
		skills[n] = mainq * pow( 2, n );
		
		// DEBUG
		if (dbug==true) cout << "DEBUG: Skill " << n << " is:	" << skills[n] << " XP" << endl;
	}
	
	// Lleno el array totalxp con los valores que se requieren
	// n=1 porque queremos sumar la xp de skills, no la de la cualificacion principal que hay en skills[0]
	for ( n=1; n<=14; n++ ) {
		acum += skills[n];
		totalxp[n] = acum;
		
		// DEBUG
		if (dbug==true) cout << "DEBUG: totalxp " << n << " is:	" << totalxp[n] << " XP" << endl;
	}
	
	
	// El ciclo del programa comienza acá. Deberiamos chequear el estado de las variables primero y resetear.
	
	do {
		// reset de variables
		err_skill=true; err_perc=true;
		
		// Limpiamos la consola... ugh... system
		system("cls");
		
		// Presentación del programa
		cout << "--------------------------------------------------\n";
		cout << "  WOT CREW SKILL RESET CALCULATOR BY PRAMALOT" << endl;
		cout << "--------------------------------------------------\n\n";
		cout << "I wrote this program to answer the question: 'How long do i have to train a crewmember\n";
		cout << "so i can reset his skills without losing the ones he already has?' \n\n";
		cout << "Simply input the number of fully trained perks/skills, then the percentage of the \n";
		cout << "perk/skill currently in training. \n\nThe program will tell you the total XP your crewmember has. \n\n";
		cout << "After that, you select a skill reset option, and the program will show you your \n";
		cout << "skills/percentage before and after the reset.\n\n";
		cout << "Then you rinse and repeat, increase/reduce the percentage, until you find the sweet spot\n";
		cout << "that conserves your hardly earned skills! \n\n";
		cout << "Currently the program assumes your crewmember has 100% in his main qualification. \n";
		cout << "Also, notice that for female crewmembers, the SOS skill doesn't count! \n\nEnjoy! \n";
		
		
		// Ciclo para volver a preguntar por skills.
		while ( err_skill==true ) {
			// pregunta por skills entrenados
			cout << "\n--------------------------------------------------\n";
			cout << "How many fully trained (100%) skills does your crewman have?\nSkills: ";
			cin >> ft;
			
			// if para restringir maximo de skills 100% a 13
			if ( ft>=14 ) {
				cout << "\nUnicuminati crew? yea right... \nOn a seriuous note, please try 13 skills or lower. Sorry! \n";
				err_skill = true; // esto hace que vuelva a preguntar x skills
			} else {
				err_skill = false;
				
				// Ciclo para preguntar por el porcentaje de skill parcialmente entrenado
				while ( err_perc==true ) {
					// Pregunta por porcentaje de skill parcialmente entrenado
					cout << "\n--------------------------------------------------\n";
					cout << "What's the percentage of his " << ft+1 << "th skill?\nPercentage: ";
					cin >> percent;
					
					// if para restringir porcentaje de skill parcial a 99% o menos
					if ( percent == 100 ) {
						cout << "\nThat's a fully trained skill! Percentage must be 99 or less. Sorry! \n";
						err_perc = true;
					} else if ( percent > 100 ) {
						cout << "\nOP skills..? you mistyped my friend. Try 99 or less... Such is that :P \n";
						err_perc = true;
					} else {
						err_perc = false; //podemos seguir
						cout << endl << "Total XP in fully trained skills is " << totalxp[ft] << endl;
						
						// calcular la XP en el skill parcial
						// reseteo acum
						acum = 0;
						for ( n=0; n<percent; n++ ) {
							xpartial = pow(2,(float)ft+2) * 25 * pow(100,(float)n/100 );
							acum += round(xpartial);
							
							// DEBUG
							if (dbug==true) cout << "DEBUG: Required XP to increase skill " << ft+1 << " from " << n << "% to " << n+1 << "% 	-> " << round(xpartial) << endl;
						}
						
						// publicar el valor de XP en el skill parcial
						cout << "Total XP in partially trained skill " << ft+1 << " is " << acum << endl;
						
						// Calculamos la XP total.Se suma acum al total de XP segun los skills que tiene el usuario (los cuales ya calculamos en totalxp)
						fullxp = acum + totalxp[ft];
						
						// publicar el valor de la XP total
						cout << "Total XP in ALL skills is " << fullxp << endl;
						
						// Menu de opciones para el reset
						cout << "\n--------------------------------------------------\n";
						cout << "Type a number (1-3) to select a skill reset option:" << endl;
						cout << "1. Free (-20% XP penalty)" << endl;
						cout << "2. 20K credits (-10% XP penalty)" << endl;
						cout << "3. 200 Gold (No XP penalty)\n>> ";
						cin >> menu;
						
						// Procedimiento para cada opcion
						switch(menu) {
							case 1: // Free (-20% XP penalty)
								fullxp -= (fullxp * 0.2);
								break;
							case 2: // 20K credits (-10% XP penalty)
								fullxp -= (fullxp * 0.1);
								break;
							case 3: // 200 Gold (No XP penalty)
								fullxp -= (fullxp * 0);
								break;
						}
						
						cout << "--------------------------------------------------\n\n";
						cout << "Total XP after reset is " << fullxp << endl;
						cout << "You lose: " << acum+totalxp[ft]-fullxp << " XP" << endl << endl;
						cout << "Before reset: \nComplete skills: " << ft << " - Partial skill percentage: " << percent << "%" << endl << endl;
						
						// Contar el n° de skills luego del reset. Restar a fullxp los valores en skills de manera sucesiva
						n=0;
						while ( fullxp > skills[n+1] ) {
							fullxp -= skills[n+1];
							n++;
							
							// DEBUG
							if (dbug==true) cout << "DEBUG: Skill " << n << " is counted. Fullxp left: " << fullxp << endl;
						}
						
						// Salvar el valor de n en ft_reset
						ft_reset = n; // Skills al 100% luego del reset
						
						// DEBUG
						if (dbug==true) cout << "DEBUG: Fullxp left for next calculation: " << fullxp << endl;
						
						// Calcular el porcentaje de skill correspondiente a la XP que queda en fullxp
						// resetear variables que entran en juego:
						acum=0; xpartial=0; n=0;
						
						// DEBUG
						if (dbug==true) cout << "DEBUG: acum:" << acum << " xpartial:" << xpartial << " n:" << n << endl;
						
						while ( acum <= fullxp ) {
							xpartial = pow(2,(float)ft_reset+2) * 25 * pow(100,(float)n/100 );
							acum += round(xpartial);
							if (acum <= fullxp) n++;
							
							// DEBUG
							if (dbug==true) cout << "Skill " << ft_reset+1 << ": " << n << "% to " << n+1 << "% ->	" << xpartial << " - acum reads: "<< acum << endl;
						}
						
						// resultados del reset
						cout << "After reset: \nComplete skills: " << ft_reset << " - Partial skill percentage: " << n << "%" << endl << endl;
						
						// preguntamos si queremos continuar
						cout << "--------------------------------------------------\n";
						cout << "We are finished. Do you wish to start over?" << endl << "Type Y/N and press Enter" << endl << ">> ";
						cin >> usr_cont;
						
						// Si pone Y o y seguimos. de lo contrario chao.
						if ( !cin.fail() && ( usr_cont=='Y' || usr_cont=='y' ) ) {
							continue_calc = true;
						} else {
							continue_calc = false;
						}
					}
				}
			}
		}
	} while ( continue_calc == true );
}
