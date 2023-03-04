using Interop;
using System.Text;

namespace TarkovInventoryAssistant_Server
{

    /*
     This namespace contains helper classes that may be useful elsewhere in the code.  
     */
    namespace Utilities
    {
        // Because Tuple isn't mutable!
        public class Pair<T1, T2>
        {
            public Pair(T1 first, T2 second)
            {
                First = first;
                Second = second;
            }

            public T1 First { get; set; }
            public T2 Second { get; set; }
        }

        // Object used to track the total value of DetectionResults.
        public class TotalValueCounter
        {
            public TotalValueCounter()
            {
                m_unitValueMap = new Dictionary<string, int>();
            }

            public void addResultEntry(DetectionResultMarshal drm) {

                string unit;
                int value;
                if (drm.sellOnFlea)
                {
                    int nullIndex = Array.IndexOf(drm.fleaUnit, (byte)0);
                    string fleaUnit = Encoding.UTF8.GetString(drm.fleaUnit, 0, nullIndex);
                    unit = fleaUnit;
                    value = drm.fleaprice;
                }
                else
                {
                    int nullIndex = Array.IndexOf(drm.traderUnit, (byte)0);
                    string traderunit = Encoding.UTF8.GetString(drm.traderUnit, 0, nullIndex);
                    unit = traderunit;
                    value = drm.fleaprice;
                }

                if (m_unitValueMap.ContainsKey(unit))
                {
                    m_unitValueMap[unit] += value;
                }
                else
                {
                    m_unitValueMap[unit] = value;
                }
            
            }
            // Formats units and values into a nicely displayable string.
            public string getValueString()
            {
                StringBuilder sb = new StringBuilder();
                foreach (string unit in m_unitValueMap.Keys)
                {
                    sb.Append(unit + m_unitValueMap[unit].ToString("N0") + "  ");
                }

                return sb.ToString();
            }


            // Unit : cumulative value
            private Dictionary<string, int> m_unitValueMap;

        }



    }

}
