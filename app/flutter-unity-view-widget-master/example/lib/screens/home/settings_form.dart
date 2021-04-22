import 'package:flutter/material.dart';
import 'package:flutter_unity_widget_example/models/athlete.dart';
import 'package:flutter_unity_widget_example/models/user.dart';
import 'package:flutter_unity_widget_example/services/database.dart';
import 'package:flutter_unity_widget_example/shared/constants.dart';
import 'package:flutter_unity_widget_example/shared/loading.dart';
import 'package:provider/provider.dart';

class SettingsForm extends StatefulWidget {
  @override
  _SettingsFormState createState() => _SettingsFormState();
}

class _SettingsFormState extends State<SettingsForm> {

  final _formKey = GlobalKey<FormState>();

  // form values
  String _currentName;
  String _currentSport;
  String _currentAge;

  @override
  Widget build(BuildContext context) {

    final user = Provider.of<User>(context);
    final athletes = Provider.of<Athlete>(context);
    int i = 0;

    for (var player in athletes.players) {
      print(athletes.players[i]);
      i++;
    }

    // return StreamBuilder<UserData>(
    //   stream: DatabaseService(uid: user.uid).userData,
    //   builder: (context, snapshot) {
    //     if(snapshot.hasData) {

    //       UserData userData = snapshot.data;

          return Form(
            key: _formKey,
            child: Column(
              children: [
                Text(
                  'Add a new player:',
                  style: TextStyle(fontSize: 18.0),
                ),
                SizedBox(height: 20.0),
                TextFormField(
                  //initialValue: userData.name,
                  decoration: textInputDecoration.copyWith(hintText: 'Name'),
                  validator: (val) => val.isEmpty ? 'Please enter a name' : null,
                  onChanged: (val) => setState(() => _currentName = val),
                ),
                SizedBox(height: 20.0),
                TextFormField(
                  //initialValue: userData.sport,
                  decoration: textInputDecoration.copyWith(hintText: 'Sport'),
                  validator: (val) => val.isEmpty ? 'Please enter a sport' : null,
                  onChanged: (val) => setState(() => _currentSport = val),
                ),
                SizedBox(height: 20.0),
                TextFormField(
                  //initialValue: userData.sport,
                  decoration: textInputDecoration.copyWith(hintText: 'Age'),
                  validator: (val) => val.isEmpty ? 'Please enter an age' : null,
                  onChanged: (val) => setState(() => _currentAge = val),
                ),
                // drop down
                // slider
                RaisedButton(
                  color: Colors.pink[400],
                  child: Text(
                    'Update',
                    style: TextStyle(color: Colors.white),
                  ), 
                  onPressed: () async {
                    if(_formKey.currentState.validate()) {
                      await DatabaseService(uid: user.uid).addNewAthlete(
                        [{'name' : _currentName, 'sport' : _currentSport , 'age' : _currentAge}],
                      );
                      Navigator.pop(context);
                    }
                  },
                )
              ],
            ),
          );
  //       } else {
  //         return Loading();
  //       }
        
  //     }
  //   );
   }
}