#ifndef CGPSPOS_H
#define CGPSPOS_H

class CGpsPos {
public:

CGpsPos(void);
~CGpsPos(void);

inline void Longitude(float longitude) { m_longitude = longitude; };
inline float Longitude(void) { return m_longitude; };

inline void Latitude(float latitude) { m_latitude = latitude; };
inline float Latitude(void) { return m_latitude; };

inline void Alt(float alt) { m_alt = alt; };
inline float Alt(void) { return m_alt; };

inline void FixIndicator(int fix_indicator) { m_fix_indicator = fix_indicator; };
inline int FixIndicator(void) { return m_fix_indicator; };


private:

float m_latitude;
float m_longitude;
float m_alt;

int m_fix_indicator;


};


#endif
