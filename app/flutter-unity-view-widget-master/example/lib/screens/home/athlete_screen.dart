import 'package:flutter/material.dart';
import 'package:flutter_unity_widget_example/models/athlete.dart';
import 'package:flutter_unity_widget_example/models/user.dart';
import 'package:flutter_unity_widget_example/screens/home/athlete_tile.dart';
import 'package:flutter_unity_widget_example/screens/home/settings_form.dart';
import 'package:flutter_unity_widget_example/services/database.dart';
import 'package:flutter_unity_widget_example/shared/loading.dart';
import 'package:provider/provider.dart';

class AthleteList extends StatefulWidget {
  @override
  _AthleteListState createState() => _AthleteListState();
}

class _AthleteListState extends State<AthleteList> {

  @override
  Widget build(BuildContext context) {

    final athletes = Provider.of<List<Athlete>>(context);

    // function for showing a pop up settings panel
    void _showSettingsPanel() {
      showModalBottomSheet(context: context, builder: (context){
        return Container(
          padding: EdgeInsets.symmetric(vertical: 20.0, horizontal: 60.0),
          child: SettingsForm(),
        );
      });
    }

    return Scaffold(
      backgroundColor: Color(0xff5d73a5),
      appBar: AppBar(
        title: Text('Players'),
        backgroundColor: Color(0xff0c1423),
        elevation: 0.0,
        actions: <Widget>[
          FlatButton.icon(
            icon: Icon(Icons.settings),
            label: Text('settings'),
            textColor: Colors.white,
            onPressed: () => _showSettingsPanel(),
          )
        ]
      ),
      body: Center(
        child: ListView.builder(
          itemCount: athletes.length,
          itemBuilder: (context, index) {
            return AthleteTile(athlete: athletes[index]);
          },
        ),
      ),
    );
  } 
}

