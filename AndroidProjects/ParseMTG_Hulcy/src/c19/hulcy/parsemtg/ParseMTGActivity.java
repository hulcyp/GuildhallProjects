package c19.hulcy.parsemtg;

import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.ParseException;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Toast;

import com.parse.FindCallback;
import com.parse.Parse;
import com.parse.ParseObject;
import com.parse.ParseQuery;

public class ParseMTGActivity extends Activity {


	private static final String TAG = "MTG";
	
	// Replace these with the values from your own Parse.com app!
	private final String APPLICATION_ID = "6XPhZgWIp6OWLBh7ufODWfl5VeMwxYoZ4qZ2IJew";
	private final String CLIENT_KEY = "AMyZpPcqJS8JpAN5Av087N9chMi5cWrZqly7u9Gc";
		
	private String m_gameID;
	private String m_player1ID;
	private String m_player2ID;
	private String m_currentPhase;
	
	private int m_player1LibraryCount = 0;
	private int m_player2LibraryCount = 0;
	
	
	public boolean isNetworkAvailable()
	{
		ConnectivityManager cm = (ConnectivityManager) getSystemService( Context.CONNECTIVITY_SERVICE );
		NetworkInfo networkInfo = cm.getNetworkInfo(ConnectivityManager.TYPE_WIFI);

		// If no network is available networkInfo will be null
		// otherwise check if we are connected

		if( networkInfo != null && networkInfo.isConnected() )
			return true;

		return false;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		this.setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_PORTRAIT );
		
		if( isNetworkAvailable() )
		{
			Parse.initialize( this, APPLICATION_ID, CLIENT_KEY );
			createGame();
		}
		else
			Log.e( TAG, "No network available" );
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.parse_mtg, menu);
		return true;
	}
	
	
	public void createGame()
	{
		final ParseObject player1 = new ParseObject( "Player" );
		final ParseObject player2 = new ParseObject( "Player" );
		player1.put( "username", "jedimonky" );
		player2.put( "username", "darthmonky" );
		
		try {
			player1.save();
			player2.save();
		} catch (com.parse.ParseException e) {
			Log.e( TAG, e.getMessage() );
		}
		
		
		m_player1ID = player1.getObjectId();
		m_player2ID = player2.getObjectId();
		
		addCardsToParse();
		
		addCardToDeck( m_player1ID, "Smite", 0 );
		addCardToDeck( m_player1ID, "Advent of the Wurm", 0 );
		addCardToDeck( m_player1ID, "Call of the Conclave", 0 );
		
		addCardToDeck( m_player2ID, "Phalanx Leader", 0 );
		addCardToDeck( m_player2ID, "Favored Hoplite", 0 );
		addCardToDeck( m_player2ID, "Ethereal Armor", 0 );
		
		final ParseObject game = new ParseObject( "Game" );
		game.put( "player1_id", m_player1ID );
		game.put( "player2_id", m_player2ID );
		game.put( "current_phase", "Main Phase" );
		game.put( "turn_number", 1 );
		m_currentPhase = "Main Phase";
		
		try {
			game.save();
			m_gameID = game.getObjectId();
		} catch (com.parse.ParseException e) {
			Log.e( TAG, e.getMessage() );
		}
		
		
		addCardToLibrary( m_player1ID, "Smite", 0 );
		addCardToLibrary( m_player1ID, "Advent of the Wurm", 1 );
		addCardToLibrary( m_player1ID, "Call of the Conclave", 2 );
		m_player1LibraryCount = 3;
		
		addCardToLibrary( m_player2ID, "Phalanx Leader", 0 );
		addCardToLibrary( m_player2ID, "Favored Hoplite", 1 );
		addCardToLibrary( m_player2ID, "Ethereal Armor", 2 );
		m_player2LibraryCount = 3;
		
				
		addCardToPlayerHand( m_player1ID, "Advent of the Wurm" );
		addCardToPlayerHand( m_player2ID, "Favored Hoplite" );
	}
	
	public void addCardsToParse()
	{
		try
		{
			JSONObject manaCost = new JSONObject();
			manaCost.put( "type", "forest" );
			manaCost.put( "amount", 2 );
			manaCost.put( "type", "plains" );
			manaCost.put( "amount", 1 );
			manaCost.put( "type", "colorless" );
			manaCost.put( "amount", 1 );
			
			JSONObject types = new JSONObject();
			types.put( "primary", "Instant" );
			
			JSONObject abilities = new JSONObject();
			JSONObject ability = new JSONObject();
			ability.put( "name", "Spawn token creature" );
			ability.put( "type", "onSpawn" );
			ability.put( "tap_to_activate", false );
			abilities.put( "ability", ability );
			
			addCardToCardList( "Advent of the Wurm", manaCost, types, abilities, 0, 0);

			manaCost = new JSONObject();
			manaCost.put( "type", "plains" );
			manaCost.put( "amount", 1 );
			
			types = new JSONObject();
			types.put( "primary", "Creature" );
			types.put( "secondary", "Human Soldier" );
			
			abilities = new JSONObject();
			ability = new JSONObject();
			ability.put( "name", "Heroic" );
			ability.put( "type", "activate" );
			ability.put( "tap_to_activate", false );
			abilities.put( "ability", ability );
			
			addCardToCardList( "Favored Hoplite", manaCost, types, abilities, 1, 2);
		}
		catch( JSONException e )
		{
			Log.e( TAG, e.getMessage() );
		}
	}
	
	public void addCardToCardList( String cardName, JSONObject manaCost, JSONObject types, JSONObject abilities, int power, int toughness )
	{
		final ParseObject card = new ParseObject( "Card" );
		card.put( "name", cardName );
		card.put( "mana_cost", manaCost.toString() );
		card.put( "types", types.toString() );
		card.put( "abilities", abilities.toString() );
		card.put( "power", power );
		card.put( "toughness", toughness );
		
		try {
			card.save();
		} catch (com.parse.ParseException e) {
			Log.e( TAG, e.getMessage() );
		}
	}
	
	public void addCardToPlayerHand( String playerID, String cardName )
	{
		final ParseObject playerHand = new ParseObject( "Hand" );
				
		playerHand.put( "player_id", playerID );
		playerHand.put( "game_id", m_gameID );
		playerHand.put( "card_name", cardName );

		try {
			playerHand.save();
		} catch (com.parse.ParseException e) {
			Log.e( TAG, e.getMessage() );
		}
	}
	
	public void addCardToDeck( String playerID, String cardName, int deckID )
	{
		final ParseObject deck = new ParseObject( "Deck" );
		
		deck.put( "card_name", cardName );
		deck.put( "deck_id", deckID );
		
		deck.put( "owner_player_id", playerID );
		
		try {
			deck.save();
		} catch (com.parse.ParseException e) {
			Log.e( TAG, e.getMessage() );
		}
	}

	public void addCardToLibrary( String playerID, String cardName, int order )
	{
		final ParseObject library = new ParseObject( "Library" );
		library.put( "order_index", order );
		library.put( "player_id", playerID );
		library.put( "game_id", m_gameID );
		library.put( "card_name", cardName );

		try {
			library.save();
			Toast.makeText( getApplicationContext(), "Card added to library: " + cardName, Toast.LENGTH_SHORT ).show();
		} catch (com.parse.ParseException e) {
			Log.e( TAG, e.getMessage() );
		}
	}
	
	public void onPutCardInLibrary( View v )
	{
		addCardToLibrary( m_player1ID, "Call of the Conclave", m_player1LibraryCount );
		m_player1LibraryCount++;
		
	}
	
	public void onDrawCardClick( View v )
	{
		ParseQuery<ParseObject> query = ParseQuery.getQuery( "Library" );

		query.whereEqualTo( "game_id", m_gameID);
		query.whereEqualTo( "order_index", m_player1LibraryCount - 1 );
		
		query.findInBackground( new FindCallback<ParseObject>()
		{
			@Override
			public void done(List<ParseObject> libraryCards, com.parse.ParseException e) {
				 
				if( e == null && libraryCards.size() > 0 )
				{
					String cardName = libraryCards.get(0).getString( "card_name" );
					if( !cardName.isEmpty() )
					{
						ParseObject hand = new ParseObject( "Hand" );
						hand.put( "game_id", m_gameID );
						hand.put( "player_id", m_player1ID );
						
						hand.put( "card_name", cardName );
						Toast.makeText( getApplicationContext(), "Drew card: " + cardName  , Toast.LENGTH_SHORT ).show();
						try 
						{
							hand.save();
							libraryCards.get(0).delete();
							m_player1LibraryCount--;			
						} 
						catch (com.parse.ParseException e1) 
						{
							Log.e( TAG, e1.getMessage() );
						}
					}
				}
			}
		});
				
	}
	
	public void onPlayCardClick( View v )
	{
		ParseQuery<ParseObject> query = ParseQuery.getQuery( "Hand" );
		query.whereEqualTo( "game_id", m_gameID);
		query.findInBackground( new FindCallback<ParseObject>()
		{
			@Override
			public void done(List<ParseObject> handCards, com.parse.ParseException e) {
				 
				if( e == null && handCards.size() > 0 )
				{
					String cardName = handCards.get(0).getString( "card_name" );
					
					if( !cardName.isEmpty() )
					{
						ParseObject battlefield = new ParseObject( "Battlefield" );
						battlefield.put( "game_id", m_gameID );
						battlefield.put( "player_id_owner", m_player1ID );
						battlefield.put( "player_id_controller", m_player1ID );
						battlefield.put( "tapped", false );
						
						battlefield.put( "card_name", cardName );
						Toast.makeText( getApplicationContext(), "Played card: " + cardName  , Toast.LENGTH_SHORT ).show();
						
						ParseObject stack = new ParseObject( "Stack" );
						stack.put( "game_id", m_gameID );
						stack.put( "player_id", m_player1ID );
						stack.put( "card_name", cardName );
						stack.put( "order_on_stack", 0 );
						stack.put( "phase", m_currentPhase );					
						
						try 
						{
							stack.save();
							battlefield.save();
							handCards.get(0).delete();
							m_player1LibraryCount--;
						} 
						catch (com.parse.ParseException e1) 
						{
							Log.e( TAG, e1.getMessage() );
						}
					}
				}
			}
		});
	}
	
	public void onPlayManaClick( View v )
	{
		final ParseObject land = new ParseObject( "LandPool" );
		land.put( "player_id", m_player1ID );
		land.put( "game_id", m_gameID );
		land.put( "type", "Forest" );
		land.put( "tapped", false );
		
		try {
			land.save();
			Toast.makeText( getApplicationContext(), "Played Forest land", Toast.LENGTH_SHORT ).show();
		} catch (com.parse.ParseException e) {
			Log.e( TAG, e.getMessage() );
		}
	}
	
	public void onTapManaClick( View v )
	{
		ParseQuery<ParseObject> query = ParseQuery.getQuery( "LandPool" );
		query.whereEqualTo( "game_id", m_gameID);
		
		query.findInBackground( new FindCallback<ParseObject>() 
		{
			@Override
			public void done( List<ParseObject> lands, com.parse.ParseException e )
			{
				if( e == null )
				{
					for( ParseObject land : lands )
					{
						boolean isTapped = land.getBoolean( "tapped" );
						isTapped = !isTapped;
						land.put( "tapped", isTapped );

						try {
							land.save();
							Toast.makeText( getApplicationContext(), "Land tapped: " + isTapped, Toast.LENGTH_SHORT ).show();
						} catch (com.parse.ParseException e1) {
							Log.e( TAG, e1.getMessage() );
						};						
					}
				}
			}
		});
	}
	
	public void onExileCard( View v )
	{
		ParseQuery<ParseObject> query = ParseQuery.getQuery( "Battlefield" );

		query.whereEqualTo( "game_id", m_gameID);
		
		query.findInBackground( new FindCallback<ParseObject>() 
				{
					@Override
					public void done( List<ParseObject> cardsOnBattlefield, com.parse.ParseException e )
					{
						if( e == null )
						{
							if( cardsOnBattlefield.size() > 0 )
							{
								String cardName = cardsOnBattlefield.get(0).getString( "card_name" );
								if( !cardName.isEmpty() )
								{
									ParseObject exiledCard = new ParseObject( "Exiled" );
									exiledCard.put( "game_id", m_gameID );
									exiledCard.put( "player_id", cardsOnBattlefield.get(0).getString("player_id_owner") );
									exiledCard.put( "card_name", cardName );
									
									try 
									{
										exiledCard.save();
										cardsOnBattlefield.get(0).delete();
										Toast.makeText( getApplicationContext(), "Card exiled: " + cardName, Toast.LENGTH_SHORT ).show();
									} 
									catch (com.parse.ParseException e1) 
									{
										Log.e( TAG, e1.getMessage() );
									};
								}
							}	
						}
					}
				});
	}
	
	public void onMoveToGraveyard( View v )
	{
		ParseQuery<ParseObject> query = ParseQuery.getQuery( "Battlefield" );
		query.whereEqualTo( "game_id", m_gameID);
		
		query.findInBackground( new FindCallback<ParseObject>() 
				{
					@Override
					public void done( List<ParseObject> cardsOnBattlefield, com.parse.ParseException e )
					{
						if( e == null )
						{
							if( cardsOnBattlefield.size() > 0 )
							{
								String cardName = cardsOnBattlefield.get(0).getString( "card_name" );
								if( !cardName.isEmpty() )
								{
									ParseObject graveyard = new ParseObject( "Graveyard" );
									graveyard.put( "game_id", m_gameID );
									graveyard.put( "player_id", cardsOnBattlefield.get(0).getString("player_id_owner") );
									graveyard.put( "card_name", cardName );
									
									try 
									{
										graveyard.save();
										cardsOnBattlefield.get(0).delete();
										Toast.makeText( getApplicationContext(), "Card move to graveyard: " + cardName, Toast.LENGTH_SHORT ).show();
									} 
									catch (com.parse.ParseException e1) 
									{
										Log.e( TAG, e1.getMessage() );
									};
								}
							}	
						}
					}
				});
	}
}
