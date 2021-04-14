import 'package:cloud_firestore/cloud_firestore.dart';

class DatabaseService {

  final String uid;
  DatabaseService({this.uid});

  // collection reference
  final CollectionReference athleteCollection = Firestore.instance.collection('athletes');

  Future updateUserData(String sport, String name) async {
    return await athleteCollection.document(uid).setData({
      'sport': sport,
      'name': name,
    });
  }

}