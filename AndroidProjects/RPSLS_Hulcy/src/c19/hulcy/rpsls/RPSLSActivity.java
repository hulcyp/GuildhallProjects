package c19.hulcy.rpsls;

import java.util.Random;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class RPSLSActivity extends Activity {
	
	//Enum used to define each of the valid choices for the game RPSLS
	public enum RPSLS_Choices
	{
		ROCK( 0, "Rock", R.drawable.rock, new int[]{ 2, 3 }, new String[]{ "crushes", "crushes" } ),
		PAPER( 1, "Paper", R.drawable.paper, new int[]{ 0, 4 }, new String[]{ "covers", "disproves" } ),
		SCISSORS( 2, "Scissors", R.drawable.scissors, new int[]{ 3, 1 }, new String[]{ "decapitate", "cuts" } ),
		LIZARD( 3, "Lizard", R.drawable.lizard, new int[]{ 4, 1 }, new String[]{ "poisons", "eats" } ),
		SPOCK( 4, "Spock", R.drawable.spock, new int[]{ 0, 2 }, new String[]{ "vaporizes", "smashes" } ),
		NONE( 5, "None", 0, new int[]{}, new String[]{} );	
		
		public final int id;
		public final String strName;
		public final int[] whatIBeat;
		public final String[] howIBeat;
		public final int imgResourceID;
		
		RPSLS_Choices( int id, String strName, int imgRID, int[] what, String[] how )
		{
			this.id = id;
			this.strName = strName;
			imgResourceID = imgRID;
			this.whatIBeat = what;
			this.howIBeat = how;
		}
		
		public String doesBeat( RPSLS_Choices other )
		{
			String reasonForVictory = "";
			for( int i = 0; i < whatIBeat.length && reasonForVictory == ""; ++i )
			{
				if( whatIBeat[i] == other.id )
				{
					reasonForVictory = this.strName + " " + howIBeat[i] +" " + other.strName + ".";
				}
			}
			return reasonForVictory;
		}		
	}
	
	
	private RPSLS_Choices m_playerChoice = RPSLS_Choices.NONE;
	private RPSLS_Choices m_enemyChoice = RPSLS_Choices.NONE;
	
	private int m_numberPlayerWins = 0;
	private int m_numberEnemyWins = 0;
	private int m_numberTies = 0;
	
	private TextView m_playersChoiceTV;
	private TextView m_enemyChoiceTV;
	private TextView m_roundOutcomeTV;
	private TextView m_winLoseTieTV;
	
	private ImageView m_playerChoiceIV;
	private ImageView m_enemyChoiceIV;
	
	private Random m_randomNumGen;

	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_rpsls);
		
		m_roundOutcomeTV = (TextView)findViewById( R.id.roundOutcomeText );
		m_winLoseTieTV = (TextView)findViewById( R.id.winLoseTieText );
		m_playersChoiceTV = (TextView)findViewById(R.id.playersChoice);
		m_enemyChoiceTV = (TextView)findViewById(R.id.enemiesChoice);
		
		m_playerChoiceIV = (ImageView)findViewById(R.id.playChoiceIcon);
		m_enemyChoiceIV = (ImageView)findViewById(R.id.enemiesChoiceIcon);
	
		if( savedInstanceState != null )
		{
			m_numberPlayerWins = savedInstanceState.getInt( "num_player_wins" );
			m_numberEnemyWins = savedInstanceState.getInt( "num_enemy_wins" );
			m_numberTies = savedInstanceState.getInt( "num_ties" );
			m_roundOutcomeTV.setText( savedInstanceState.getCharSequence( "round_outcome" ) );
			int playerChoiceIdx = savedInstanceState.getInt( "player_choice_id" );
			int enemyChoiceIdx = savedInstanceState.getInt( "enemy_choice_id" );
			m_playerChoice = RPSLS_Choices.values()[ playerChoiceIdx ];
			m_enemyChoice = RPSLS_Choices.values()[ enemyChoiceIdx ];
			m_playerChoiceIV.setImageResource( m_playerChoice.imgResourceID );
			m_enemyChoiceIV.setImageResource( m_enemyChoice.imgResourceID );
			savedInstanceState.putInt( "enemy_choice_resource_id",  m_enemyChoice.imgResourceID );
		}
		
		
		
		m_randomNumGen = new Random();		
		m_playersChoiceTV.setText( getString( R.string.player ) + ": " );
		m_enemyChoiceTV.setText( getString( R.string.opponent ) + ": " );
		m_winLoseTieTV.setText( getString( R.string.winLoseTie ) + " " + m_numberPlayerWins + "/" + m_numberEnemyWins + "/" + m_numberTies );
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.rpsl, menu);
		return true;
	}
	
	@Override
	public void onSaveInstanceState( Bundle savedInstanceState )
	{
		super.onSaveInstanceState( savedInstanceState );
		savedInstanceState.putInt( "num_player_wins", m_numberPlayerWins );
		savedInstanceState.putInt( "num_enemy_wins", m_numberEnemyWins );
		savedInstanceState.putInt( "num_ties", m_numberTies );
		savedInstanceState.putCharSequence( "round_outcome", m_roundOutcomeTV.getText() );
		savedInstanceState.putInt( "player_choice_id", m_playerChoice.id );
		savedInstanceState.putInt( "enemy_choice_id",  m_enemyChoice.id );
	}
	
	private void makeEnemyChoice()
	{
		int enumIdx = m_randomNumGen.nextInt( RPSLS_Choices.NONE.id );
		m_enemyChoice = RPSLS_Choices.values()[ enumIdx ];
	}
	
	private void checkCurrentWinner()
	{
		String reasonForOutCome;
		if( m_playerChoice == m_enemyChoice )
		{
			//tie
			reasonForOutCome = getString( R.string.tie );
			++m_numberTies;
		}
		else if( ( reasonForOutCome = m_playerChoice.doesBeat( m_enemyChoice ) ) != "" )
		{
			//player wins
			reasonForOutCome += " " + getString( R.string.win );
			++m_numberPlayerWins;
		}
		else
		{
			//ai wins
			reasonForOutCome = m_enemyChoice.doesBeat(m_playerChoice);
			reasonForOutCome += " " + getString( R.string.lose );
			++m_numberEnemyWins;
		}
		
		m_playersChoiceTV.setText( getString( R.string.player ) + ":" );
		m_enemyChoiceTV.setText(getString( R.string.opponent ) + ":" );
		m_playerChoiceIV.setImageResource( m_playerChoice.imgResourceID );
		m_enemyChoiceIV.setImageResource( m_enemyChoice.imgResourceID );
		m_roundOutcomeTV.setText(reasonForOutCome);
		m_winLoseTieTV.setText( getString( R.string.winLoseTie ) + " " + m_numberPlayerWins + "/" + m_numberEnemyWins + "/" + m_numberTies );
		
		
	}
	
	private void handlePlayerSelectChoice( RPSLS_Choices choice )
	{
		m_playerChoice = choice;
		makeEnemyChoice();
		checkCurrentWinner();
	}
	
	public void onSpockClick( View v )
	{
		handlePlayerSelectChoice( RPSLS_Choices.SPOCK );
	}
	
	public void onLizardClick( View v )
	{
		handlePlayerSelectChoice( RPSLS_Choices.LIZARD );
	}
	
	public void onRockClick( View v )
	{
		handlePlayerSelectChoice( RPSLS_Choices.ROCK );
	}
	
	public void onPaperClick( View v )
	{
		handlePlayerSelectChoice( RPSLS_Choices.PAPER );
	}
	
	public void onScissorsClick( View v )
	{
		handlePlayerSelectChoice( RPSLS_Choices.SCISSORS );
	}

}
