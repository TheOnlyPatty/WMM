import 'package:flutter/material.dart';
import 'package:flutter_unity_widget_example/models/athlete.dart';

class AthleteTile extends StatelessWidget {

  final Athlete athlete;
  AthleteTile({ this.athlete });

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.only(top: 8.0),
      child: Card(
        margin: EdgeInsets.fromLTRB(20.0, 6.0, 20.0, 0.0),
        child: ListTile(
          leading: Icon(
            Icons.account_circle,
            size: 50.0,
            color: Color(0xff0c1423),
          ),
          title: Text(athlete.name),
          subtitle: Text(athlete.sport), 
        ),
      )
    );
  }
}