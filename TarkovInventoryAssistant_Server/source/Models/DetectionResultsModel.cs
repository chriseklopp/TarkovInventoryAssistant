using Interop;

namespace TarkovInventoryAssistant_Server.Models
{
    public class DetectionResultsModel
    {

        public DetectionResultsModel()
        {

        }

        public DetectionResultsModel(DetectionResultMarshal drm)
        {
            catalogImage = 69;
            sourceImage = 420;
            name = drm.name;
            dim = drm.dimHeight.ToString() + 'x' + drm.dimWidth.ToString();
            fleaPrice = drm.fleaUnit + drm.fleaprice.ToString();
            pricePerSlot = drm.fleaUnit + drm.fleaPricePerSlot.ToString();
            traderPrice = drm.traderUnit + drm.traderPrice.ToString();
            trader = drm.trader;
            parentImageID = 666;
        }

        public string? RequestId { get; set; }

        public int catalogImage { get; set; }

        public int sourceImage { get; set; }

        public string name { get; set; } = "";

        public string dim { get; set; } = "";

        public string fleaPrice { get; set; }

        public string pricePerSlot { get; set; }

        public string traderPrice { get; set; }

        public string trader { get; set; } = "";

        public int parentImageID { get; set; }

    }
}