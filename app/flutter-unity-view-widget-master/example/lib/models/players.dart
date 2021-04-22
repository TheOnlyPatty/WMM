import 'package:cloud_firestore/cloud_firestore.dart';

class Players{

  final List players; 

  Players({ this.players });

  Players.fromSnapshot(DocumentSnapshot snapshot) :
    players = snapshot.data['players'];

  Map<String, dynamic> playersItem(String name, String age, String sport) {
    return {
      'players': FieldValue.arrayUnion([
        {
          'name' : name,
          'age' : age,
          'sport' : sport,
        }
      ]),
    };
  }

}