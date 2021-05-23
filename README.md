# Geamuri-Destepte 


Cerinte de sistem

	Testat pe Ubuntu Server, pentru ca Pistache nu accepta Windows. 
	Am folosit un compilator C++. Pentru compilare am folosit g++ pentru c++17.
	Pentru instalarea Pistache, am clonat folderul git si l-am construit prin comanda build.
	

Building -> utilizand comanda Make pentru construirea aplicatiei ( make )


Rulare
	Pentru a porni serverul utilizam comanda ./smartwindows
	Serverul ar trebui sa afiseze numarul de nuclee si fire de executie folosite si nicio eroare
	Pentru a testa, deschidem un alt terminal si utilizam comanda 'curl http://localhost:9080/ready'. Ar trebui sa afiseze mesajul : "Your window is ready to set the properties!".
	
	Acum serverul ruleaza.


Functionalitatile implementate :

1. Geamuri/Oglinda retrovizoare interioara heliomata 
	
	- propertyName : heliomare 
	- value : numar intreg ce reprezinta valoare luminii
	- 3 trepte de heliomare : 
		(1 - 30 % lumina ) -> treapta 1 de heliomare (seara) 
		(30 - 60% lumina) -> treapta 2 de heliomare (dimineata) 
		(60 - 100% lumina) -> treapta 3 de heliomare 

	GET: curl -X GET http://localhost:9080/properties/heliomare
	POST: curl -X POST http://localhost:9080/properties/heliomare/:value (valoarea luminii) 
	
	Rezultat : setez treapta de heliomare 1/2/3 ( in functie de value) 
	
	
2. Oglinzi incalzite : 
	- propertyName: incalzire 
	- value : numar intreg ce reprezinta temperatura exterioara
   	- primeste ca valoare temperatura exterioara : 
		 - daca temp > 0, opreste incalzirea ( Off )
		 - daca temp < 0, porneste incalzirea ( On )
				
	GET: curl -X GET http://localhost:9080/properties/incalzire
	POST: curl -X POST http://localhost:9080/properties/incalzire/:value (temperatura exterioara)

	Rezultat : 
		- value > 0 => Off
		- value < 0 => On
											   
3. Deschiderea/ Inchiderea trapei panoramice 
	- propertyName : trapa
	- value : numar intreg ce reprezinta umiditatea

	- trapa se inchide automat doar atunci cand ploua
	- trapa se poate deschide/inchide manual
	- daca primeste o umiditate != 100, nu se schimba starea trapei (100% => ploua => trapa se inchide) 
		
	Ruta properties (inchiderea automata) :
		GET : curl -X GET http://localhost:9080/properties/trapa
		POST: curl -X POST http://localhost:9080/properties/trapa/ :value(umiditatea) 
			
	Ruta actions (trapa este inchisa/deschisa manual) :
		POST : curl -X POST http://localhost:9080/actions/trapa/ :action(open/close) 
	
	Rezultat : opened/closed
	
4. Stergatoare 
	- propertyName : stergatoare
	- value : numar intreg ce reprezinta umiditatea
	
	- stergatoarele pornesc doar atunci cand ploua 
	- daca acestea au fost pornite manual, nu se opresc automat atunci cand nu mai ploua (daca primeste o umiditate != 100, nu se schimba starea stergatoarelor) 100% => ploua => stergatoarele pornesc 
	- daca acestea au fost pornite de ploaie, se opresc automat atunci cand nu mai ploua 
	- stergatoarele pot fi pornite/oprite manual 

	
	Ruta properties (pornire/oprire automata) : 
		GET : curl -X GET http://localhost:9080/properties/stergatoare	
		POST: curl -X POST http://localhost:9080/properties/stergatoare/:value (umiditatea) 
	
	Ruta actions (stergatoarele sunt pornite/oprite manual) :
		POST : curl -X POST http://localhost:9080/actions/stergatoare/:action (open/close)
	
	Rezultat : on/off
		
	
5. Vocal
	- propertyName : vocal
	-value : numar intreg ce reprezinta nivelul de lumina

	- atentionare vocala cand se insereaza
	
	Ruta properties :
		GET :  curl -X GET http://localhost:9080/properties/vocal
		POST: curl -X POST http://localhost:9080/properties/vocal/:value (nivelul de lumina) 
	
	
	Rezultat :
		- valoarea < 40 => atentionare = noapte
		- valoare > 40 => atentionare = zi
