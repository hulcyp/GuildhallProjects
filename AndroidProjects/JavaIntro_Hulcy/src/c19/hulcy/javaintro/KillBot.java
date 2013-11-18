package c19.hulcy.javaintro;

public class KillBot extends Bot implements Comparable<KillBot>{

	enum Weapons
	{
		LASER,
		MACHINE_GUN,
		SPEAR,
		SWORD
	};
	
	private Weapons m_weapon;
	
	public KillBot( Weapons weapon, int speed) {
		super(speed);
		m_weapon = weapon;
	}
	
	public Weapons GetWeapon()
	{
		return m_weapon;
	}
	
	public void SetWeapon( Weapons weapon )
	{
		m_weapon = weapon;
	}

	@Override
	public int compareTo( KillBot other ) {
		
		return GetSpeed() - other.GetSpeed();
	}
	
}
