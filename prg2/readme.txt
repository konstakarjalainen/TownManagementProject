Valitsin teiden tietorakenteeksi erillisen structin, joka sisältää kaupunkien nimet aina aakkosjärjestyksessä:
first aiemmin kuin second. Structit ovat unordered_set roads_ tietorakenteessa osoittimina. Town struct tietorakenteeseen 
lisäsin kaupungin naapurit eli joihin menee tie ja aputiedot värin ja "pi-osoittimen" kaupungin nimen josta 
siirryttiin tähän kaupunkiin.

Operaatioissa road_cycle_route ja least_towns_route käytin järjestelymuistirakenteena deque:ta, jolloin oli tehokasta 
poistaa ja lisätä alkioita rakenteen alusta ja lopusta, verrattuna vectoriin. Any_route ja shortest_route operaatioissa 
käytin järjestelytietorakenteena vector:ia, koska tarpeena ei ollut kuin poistaa ja lisätä alkioita tietorakenteen lopusta.
Käytin ratkaisuissa kurssin graafit-viikolta esitettyjä algoritmeja.


