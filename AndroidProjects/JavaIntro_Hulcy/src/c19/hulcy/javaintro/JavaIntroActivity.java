package c19.hulcy.javaintro;

import java.util.ArrayList;
import java.util.Collections;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;

public class JavaIntroActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_java_intro);
        
        // Task 1
        //
        // Stores a single 16-bit unicode character. Ranges: 0 to 65,535 or '\u0000'  to \uffff' 
        char testChar = 'h';
        // Stores a single 16-bit signed two's compliment integer. Range: -32,768 to 32,767 
        short testShort = 3;
        // Stores a single 32-bit signed two's compliment integer. Range: -2^31 to 2^31 - 1
        int testInt = 5;
        // Stores a single 8-bit signed two's compliment integer. Range: -128 to 127
        byte testByte = 1;
        // Stores a single 64-bit two's compliment integer. Range: -2^63 to 2^63 - 1
        long testLong = 1000;
        // Stores a single-precision 32-bit IEEE 754 floating point number. Range: 2^-129 to (2 - 2-23) * 2127 
        float testFloat = 1.0f;
        // Stores a doulbe-precision 64-bit IEEE 754 floating point number. Range: 2-1074 to (2 - 2-52) * 21023
        double testDouble = 10.0;
        // It's values can be either true or false. The data type represents a single bit but it's size in memory is not precisely defined
        boolean testBool = false;
        
        // Task 2
        //
        String string1 = "Test";
        String string2 = "test";
        
        if( string1.compareTo( string2 ) != 0 )
        {
        	Log.d( "String Compare", "They are not equal when compared including case.");        	
        }
        if( string1.compareToIgnoreCase( string2 ) == 0 )
        {
        	Log.d( "String Compare", "They are equal when ignoring case" );        	
        }
        
        
        //Task 3
        //
        String catsString = "I love cats!";        
        
        if( catsString.contains("cats") )
        {
        	String dogString = catsString.replace("cats", "dogs");
        	Log.d("String Modification", dogString );
        }
        
        //Task 4
        //
        String weirdString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for( int i = 0; i < weirdString.length(); ++i )
        {
        	Log.d("String Stepping", String.valueOf( weirdString.charAt(i) ) );
        }
        
        //Task 5
        //
        String numberString = "0 1 2 3 4 5 6 7 8 9";
        String[] numberArray = numberString.split(" ");
        
        //Task 6
        //
        ArrayList<Integer> intList = new ArrayList<Integer>();
        for( int i = 0; i < numberArray.length; ++i )
        {
        	intList.add( Integer.parseInt( numberArray[i] ) );   	
        }
        
        for( int i = 0; i < intList.size(); ++i )
        {
        	Log.d("Int Conversion", String.valueOf( intList.get(i) ) );
        }
        
        //Task 13
        //
        ArrayList<KillBot> killBots = new ArrayList<KillBot>();
        killBots.add( new KillBot( KillBot.Weapons.LASER, 5 ) );
        killBots.add( new KillBot( KillBot.Weapons.MACHINE_GUN, 3 ) );
        killBots.add( new KillBot( KillBot.Weapons.SPEAR, 1 ) );
        killBots.add( new KillBot( KillBot.Weapons.SWORD, 15 ) );
        
        //Task 15
        //
        Collections.sort( killBots );
        
        for( int i = 0; i < killBots.size(); ++i )
        {
        	Log.d("Kill Bots", "Speed: " + Integer.toString( killBots.get(i).GetSpeed() ) );
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.java_intro, menu);
        return true;
    }
    
}
