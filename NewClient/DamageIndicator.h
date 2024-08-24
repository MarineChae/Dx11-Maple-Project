#pragma once

class DamageObject;
class DamageIndicator
{
private:

	std::vector<std::shared_ptr<DamageObject>> m_vPrintDamage;
	bool m_bisValid;
	float m_fRenderCnt;
public:
	bool Init();
	bool Frame(TVector3 pos, int damage);
	bool Render();

	void SetIsValid(bool valid)  { m_bisValid = valid; };
	bool GetIsValid() const { return m_bisValid; };

	void StartPrintDamage();

public:
	DamageIndicator();
	~DamageIndicator();
};

