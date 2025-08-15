
import { SeatCraftAppPicker } from './types'

export let SeatCraftAppPickerModule: SeatCraftAppPicker;

export const setSeatCraftAppPickerModule = (module: SeatCraftAppPicker) => {
    SeatCraftAppPickerModule = module;
};

export const getSeatCraftAppPickerModule = () => SeatCraftAppPickerModule;

export const SeatCraftAppPickerBind = (module: SeatCraftAppPicker) => {
    setSeatCraftAppPickerModule(module)
}
