all:prot conf sock db bus user rela mess photo dbt cln

prot:
	cd ./proto && make
conf:
	g++ -c -o conf.o Config.cpp
sock:
	g++ -c -o sock.o Socket.cpp
db:
	g++ -c -o db.o DbManager.cpp
bus:
	g++ -c -o bus.o BusManager.cpp
	g++ -o bustest bus.o bus_test.cpp
user:
	g++ -c -o userI.o UserInfo.cpp
	g++ -c -o userM.o UserManager.cpp
	g++ -o usersvr proto/*.o *.o user_svr.cpp -lprotobuf -lmysqlclient
rela:
	g++ -c -o relaI.o RelationInfo.cpp
	g++ -c -o relaM.o RelationManager.cpp
mess:
	g++ -c -o mess.o MessageManager.cpp
photo:
	g++ -c -o photo.o PhotoManager.cpp
dbt:
	g++ -o dbtest proto/*.o db.o userI.o userM.o dbmain.cpp -lprotobuf -lmysqlclient
svr:
	g++ -o server_ssp proto/*.o *.o main.cpp -lprotobuf -lmysqlclient
cln:
	g++ -o client_ssp proto/*.o conf.o sock.o 2.client.cpp -lprotobuf
clean:
	rm -f ./proto/*.o ./proto/*.pb.cc ./proto/*.pb.h *.o dbtest server_ssp client_ssp