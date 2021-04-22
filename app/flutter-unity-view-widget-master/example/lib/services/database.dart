import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter_unity_widget_example/models/athlete.dart';
import 'package:flutter_unity_widget_example/models/user.dart';

class DatabaseService {

  final String uid;
  DatabaseService({this.uid});

  // collection reference
  final CollectionReference athleteCollection = Firestore.instance.collection('athletes');
  final CollectionReference coachCollection = Firestore.instance.collection('trainers');

  Future updateUserData(String sport, String name) async {
    return await athleteCollection.document(uid).setData({
      'sport': 'Golf',
      'name': 'Phil',
    });
  }

  // test function
  Future addNewAthlete(String name, String age, String sport) async {
    //return await coachCollection.document(uid).collection('athletes').document(name).setData({
    return await athleteCollection.document(uid).updateData({
      'players': [{'name': name, 'age': age, 'sport': sport}],
    });
  }

  // add new entry with user defined key values (just need to make the value an array of strings)
  Future addNewEntry(String key, String value) async {
    return await athleteCollection.document(uid).updateData({
      key : value,
    });
  }

  // athlete list from snapshot
  List<Athlete> _athleteListFromSnapshot(QuerySnapshot snapshot) {
    return snapshot.documents.map((doc){
      return Athlete(
        name: doc.data['name'] ?? '',
        sport: doc.data['sport'] ?? '',
        age: doc.data['age'] ?? '',
      );
    }).toList();
  }

  // user data from snapshot
  UserData _userDataFromSnapShot(DocumentSnapshot snapshot) {
    return UserData(
      uid: uid,
      name: snapshot.data['name'] ?? '',
      sport: snapshot.data['sport'] ?? '',
    );
  }

  // get database stream
  Stream<List<Athlete>> get athletes {
    return athleteCollection.snapshots().map(_athleteListFromSnapshot);
  }

  // get user doc stream
  Stream<UserData> get userData {
    return athleteCollection.document(uid).snapshots().map(_userDataFromSnapShot);
  }

}