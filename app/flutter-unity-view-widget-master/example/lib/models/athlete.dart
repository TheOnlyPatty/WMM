import 'package:cloud_firestore/cloud_firestore.dart';

class Athlete {

    final String name;
    final String sport;
    final String age;
    final List players;

    Athlete({ this.name, this.sport, this.age, this.players});

}